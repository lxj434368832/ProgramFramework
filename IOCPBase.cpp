#include "stdafx.h"
#include "IOCPBase.h"
#include "IOCPModule.h"
#include "ConnectManage.h"
#include "ServerManage.h"
#include "ClientManage.h"
#include "INetInterface.h"
#include <WS2tcpip.h>
#include <assert.h>

IOCPBase::IOCPBase(INetInterface *pNet):
	m_pNetInterface(pNet),
	m_hIOCompletionPort(INVALID_HANDLE_VALUE),
	m_rscSocketContext(10),
	m_rscIoContext(1000)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;
	m_pListenSocketContext = nullptr;
	if (m_pNetInterface)
	{
		m_pNetInterface->SetNetworkCallback(std::bind(&IOCPBase::Send, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			std::bind(&IOCPBase::Disconnect, this, std::placeholders::_1));
	}
}

IOCPBase::~IOCPBase()
{
	m_pNetInterface = nullptr;
}

bool IOCPBase::InitIOCP(unsigned uThreadCount)
{
	if (0 == uThreadCount)
	{
		uThreadCount = IOCPModule::Instance()->GetProcessorCount() * 2 - 1;
	}

	m_hIOCompletionPort = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == m_hIOCompletionPort)
	{
		MLOG("创建完成端口失败，错误码：%d", GetLastError());
		return false;
	}

	m_aThreadList = new HANDLE[uThreadCount];
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_aThreadList[i] = ::CreateThread(NULL, 0, WorkerThread, this, 0, 0);
		if (NULL == m_aThreadList[i])
		{
			MLOG("创建IOCP线程失败，错误码:%d", GetLastError());
			return false;
		}
	}
	m_uThreadCount = uThreadCount;

	return true;
}

bool IOCPBase::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = m_rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
		{
			m_pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
			{
				MLOG("创建Server监听socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		}

		//SO_REUSEADDR允许同一port上启动同一服务器的多个实例(多个进程)。但每个实例绑定的IP地址是不能相同的。
		//int nOpt = 1;
		//setsockopt(m_pListenSocketContext->m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(nOpt));	//重用地址

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if (IOCPModule::Instance()->Bind(m_pListenSocketContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(m_pListenSocketContext->m_socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, m_hIOCompletionPort)) break;

		//投递接受操作
		for (unsigned i = 0; i < iMaxConnectCount; i++)
		{
			if (false == (bRet = PostAcceptEx(m_pListenSocketContext->m_socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		RELEASE_SOCKET(m_pListenSocketContext->m_socket);
		m_rscSocketContext.put(m_pListenSocketContext);
		m_pListenSocketContext = nullptr;
	}
	return bRet;
}

unsigned IOCPBase::StartConnect(std::string ip, u_short port, int iRecnnt, const char* data, int len)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = m_rscSocketContext.get();
	do
	{
		PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
		pIO->m_wParam = iRecnnt;

		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				MLOG("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		}

		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, m_hIOCompletionPort)) break;
		pIO->m_socket = pSkContext->m_socket;

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if(IOCPModule::Instance()->Bind(pIO->m_socket, (LPSOCKADDR)&addrLocal)) break;

		SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
		pSrvAddr->sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &pSrvAddr->sin_addr);
		pSrvAddr->sin_port = htons(port);

		bRet = PostConnectEx(pIO, (LPSOCKADDR)pSrvAddr);

	} while (0);

	if (bRet)
	{
		m_lckConnectList.lock();
		m_mapConnectList[pSkContext->m_socket] = pSkContext;
		m_lckConnectList.unlock();
		return pSkContext->m_socket;
	}
	else
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		m_rscSocketContext.put(pSkContext);
		return 0;
	}
}

DWORD WINAPI IOCPBase::WorkerThread(LPVOID lpParameter)
{

	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	IOCPBase *pThis = (IOCPBase*)lpParameter;
	MLOG("线程:%d开始运行。", ::GetCurrentThreadId());

	while (true)
	{
		int iResult = 0;
		DWORD dwBytesTransfered = 0;
		LPOVERLAPPED pOverlapped = NULL;
		BOOL bSuccess = ::GetQueuedCompletionStatus(pThis->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITY);

		if (FALSE == bSuccess)
		{
			iResult = ::GetLastError();
		}

		if (nullptr == pOverlapped)
		{
			if (ERROR_ABANDONED_WAIT_0 == iResult)//735L
				MLOG("完成端口句柄:%d被人为地关闭了!", pThis->m_hIOCompletionPort);
			else
				MLOG("完成端口发生了错误，错误码:%d ", iResult);

			MLOG(",线程:%d将退出！", ::GetCurrentThreadId());
			break;
		}

		if (nullptr == pSocketContext)
		{
			MLOG("线程:%d正常退出！", ::GetCurrentThreadId());
			break;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		pThis->HandServerOperate(iResult, pSocketContext, pIO, dwBytesTransfered);

	}

	return 0;
}

void IOCPBase::HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
		DoAccept(iResult, pSkContext, pIO);
		break;
	case EOP_CONNECT:
		DoAccept(iResult, pSkContext, pIO);
		break;

	case EOP_DISCONNECT:
		DoDisconnect(iResult, pSkContext, pIO);
		break;

	case EOP_RECEIVE:
		DoReceive(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	case EOP_SEND:
		DoSend(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	default:
		MLOG("未知的服务操作码：%d", pIO->m_oprateType);
		MAssert(false);
		break;
	}
}

bool IOCPBase::PostConnectEx(PER_IO_CONTEXT* pIO, SOCKADDR* pSrvAddr)
{
	pIO->m_oprateType = EOP_CONNECT;
	return 0 == IOCPModule::Instance()->ConnectEx(pIO, pSrvAddr);
}

void IOCPBase::DoConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (0 == iResult)
	{
		//通知服务端连接
		m_pNetInterface->AddUser((unsigned)pIO->m_socket);
		PostReceive(pSkContext, pIO);
	}
	else
	{
		MLOG("投递ConnectEx失败，错误码：%d", iResult);
		m_lckConnectList.lock();
		m_mapConnectList.erase(pSkContext->m_socket);
		m_lckConnectList.unlock();

		RELEASE_SOCKET(pSkContext->m_socket);
		//m_rscSocketContext();
	}
}

bool IOCPBase::PostAcceptEx(SOCKET listenSocket)
{
	if (INVALID_SOCKET == listenSocket)
	{
		MLOG("listenSocket无效！");
		return false;
	}

	PER_SOCKET_CONTEXT *pSocketContext = m_rscSocketContext.get();
	pSocketContext->Reset();

	if (INVALID_SOCKET == pSocketContext->m_socket)
	{
		if (INVALID_SOCKET == (pSocketContext->m_socket = IOCPModule::Instance()->Socket()))
		{
			MLOG("创建socket失败，错误码：%d", WSAGetLastError());
			return false;
		}
		MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pSocketContext, m_hIOCompletionPort));
	}

	PER_IO_CONTEXT *pIO = &pSocketContext->m_ReceiveContext;
	pIO->m_socket = pSocketContext->m_socket;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//失败
	{
		RELEASE_SOCKET(pSocketContext->m_socket);
		m_rscSocketContext.put(pSocketContext);
		return false;
	}
	else
	{
		m_lckConnectList.lock();
		m_mapConnectList[pSocketContext->m_socket] = pSocketContext;
		m_lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	PER_SOCKET_CONTEXT *pClientSkContext = nullptr;
	m_lckConnectList.lock();
	if (m_mapConnectList.find(pIO->m_socket) != m_mapConnectList.end())
	{
		pClientSkContext = m_mapConnectList[pIO->m_socket];
	}
	m_lckConnectList.unlock();

	if (0 == iResult)	//投递AcceptEx成功
	{
		SOCKADDR_IN *pClientAddr = nullptr;
		IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pClientAddr);
		char strIP[255];	inet_ntop(AF_INET, &pClientAddr->sin_addr, strIP, 255);
		MLOG("客户端 %s:%d连入,用户套接字：%d", strIP, ntohs(pClientAddr->sin_port), pIO->m_socket);

		if (pClientSkContext)
		{
			memcpy(&(pClientSkContext->m_clientAddr), pClientAddr, sizeof(SOCKADDR_IN));
		}
		else
		{
			MLOG("pClientSkContext为空，发生错误！");
			assert(pClientSkContext);
		}
		//通知服务端连接
		m_pNetInterface->AddUser((unsigned)pIO->m_socket);
		PostReceive(pClientSkContext, pIO);
		PostAcceptEx(pSkContext->m_socket);

	}
	else
	{
		MLOG("投递AcceptEx失败，错误码：%d", iResult);
		if (pClientSkContext)
		{
			m_lckConnectList.lock();
			m_mapConnectList.erase(pClientSkContext->m_socket);
			m_lckConnectList.unlock();

			int idxLock = pClientSkContext->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
			m_aLckSocketContext[idxLock].lock();
			RELEASE_SOCKET(pClientSkContext->m_socket);
			m_aLckSocketContext[idxLock].unlock();

			m_rscSocketContext.put(pClientSkContext);
		}
	}
}

void IOCPBase::Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	if (nullptr == data || 0 == uLength)
	{
		MLOG("发送的数据为空！");
		return;
	}

	if (uLength > MAX_RCV_BUF_LEN - sizeof(PackHeader))
	{
		MLOG("需要发送数据(%d)大于发送缓冲区(%d),不能发送数据！", uLength, MAX_RCV_BUF_LEN - sizeof(PackHeader));
		return;
	}

	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	int idxLock = uUserKey % SOCKET_CONTEXT_LOCK_COUNT;

	m_lckConnectList.lock();
	if (m_mapConnectList.find(uUserKey) != m_mapConnectList.end())
	{
		pSkContext = m_mapConnectList.at(uUserKey);
	}
	m_lckConnectList.unlock();

	if (nullptr == pSkContext)
	{
		MLOG("发送数据失败，不存在当前用户:%d", uUserKey);
		return;
	}

	MAutoLock lck(&m_aLckSocketContext[idxLock]);
	PackSendData(pSkContext, uMsgType, data, uLength);
	return;

}

void IOCPBase::PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length)
{
	unsigned uLeftLen = length;
	unsigned uCurrentCpyLen = 0;

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		MLOG("当前用户:%d已断开，停止发送数据", pSkContext->m_socket);
		return;
	}

	for (int i = 0; 0 != uLeftLen; i++)
	{
		if (uLeftLen < 0)
		{
			MLOG("发送数据逻辑出现错误，uLeftLen小于0！");
			break;
		}

		PER_IO_CONTEXT *pIO = m_rscIoContext.get();
		pIO->m_socket = pSkContext->m_socket;
		if (0 == i)
		{
			PackHeader head;
			unsigned uHeadSize = sizeof(PackHeader);
			head.uMsgType = uMsgType;
			head.ulBodyLength = length;
			memcpy(pIO->m_szBuffer, (char*)&head, uHeadSize);
			uCurrentCpyLen = min(uLeftLen, MAX_BUF_LEN - uHeadSize);
			memcpy(pIO->m_szBuffer + uHeadSize, data, uCurrentCpyLen);
			pIO->m_uDataLength = uCurrentCpyLen + uHeadSize;

		}
		else
		{
			uCurrentCpyLen = min(uLeftLen, MAX_BUF_LEN);
			memcpy(pIO->m_szBuffer, data, uCurrentCpyLen);
			pIO->m_uDataLength = uCurrentCpyLen;
		}

		pIO->m_wsaBuf.buf = pIO->m_szBuffer;
		pIO->m_wsaBuf.len = pIO->m_uDataLength;
		if (false == PostSend(pSkContext, pIO))
		{
			MLOG("发送数据失败，还剩%d byte数据未发送！", uLeftLen);
			break;
		}

		uLeftLen -= uCurrentCpyLen;
		data += uCurrentCpyLen;
	}

}

bool IOCPBase::PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bRet = true;
	if (1 == pSkContext->m_iDisconnectFlag)
	{
		MLOG("用户已经断开连接:%d,将停止发送数据！", pIO->m_socket);
		bRet = false;
	}
	else if (nullptr == pIO)	//发送下条IO
	{
		if (pSkContext->m_queueIoContext.empty())
		{
			pSkContext->m_iSendPendingFlag = 0;
		}
		else
		{
			pIO = pSkContext->m_queueIoContext.front();
			pSkContext->m_queueIoContext.pop();
			if (IOCPModule::Instance()->Send(pIO))	//发送失败的处理
			{
				PostDisconnectEx(pSkContext, pIO);
				bRet = false;
			}
		}
	}
	else
	{
		if (pSkContext->m_queueIoContext.empty())
		{
			//投递第一个发送
			pSkContext->m_iSendPendingFlag = 1;
			if (IOCPModule::Instance()->Send(pIO))	//发送失败的处理
			{
				PostDisconnectEx(pSkContext, pIO);
				bRet = false;
			}
		}
		else
		{
			//将当前IO放入发送队列
			pSkContext->m_queueIoContext.push(pIO);
		}
	}
	return bRet;
}

void IOCPBase::DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	MLOG("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("网络的另一端已下线！");
		else
			MLOG("发送数据失败，错误码：%d", iResult);

		PostDisconnectEx(pSkContext, pIO);
	}
	else if (0 == dwBytesTransfered)	//代表服务器已断开此socket连接
	{
		MLOG("网络的另一端已断开socket：%d的连接！", pIO->m_socket);

		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		int idxLock = pIO->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
		pIO->Reset();
		m_rscIoContext.put(pIO);
		m_aLckSocketContext[idxLock].lock();
		PostSend(pSkContext, nullptr);
		m_aLckSocketContext[idxLock].unlock();

	}
}

void IOCPBase::Disconnect(unsigned uUserKey)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	PER_IO_CONTEXT *pIO = nullptr;

	m_lckConnectList.lock();
	if (m_mapConnectList.end() != m_mapConnectList.find(uUserKey))
	{
		pSkContext = m_mapConnectList.at(uUserKey);
		pIO = m_rscIoContext.get();
	}
	m_lckConnectList.unlock();

	if (pSkContext && pIO)
	{
		PostDisconnectEx(pSkContext, pIO);
	}
}

void IOCPBase::PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bHandled = false;
	int idxLock = pIO->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&m_aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			//已经处理过断开连接了,回收IO资源
			pIO->Reset();
			m_rscIoContext.put(pIO);
			return;
		}
		else
		{
			//处理断开连接
			pSkContext->m_iDisconnectFlag = 1;
			//清理发送队列
			while (false == pSkContext->m_queueIoContext.empty())
			{
				PER_IO_CONTEXT *pIOTemp = pSkContext->m_queueIoContext.front();
				m_rscIoContext.put(pIOTemp);
				pSkContext->m_queueIoContext.pop();
			}
		}
	}

	//通知上层接口关闭连接
	m_pNetInterface->DeleteUser((unsigned)pSkContext->m_socket);
	::shutdown(pSkContext->m_socket, SD_BOTH);		//此时发送和接收IO应该会返回错误
	MLOG("关闭socket:%d的连接。", pSkContext->m_socket);

	//从连接列表中清除
	m_lckConnectList.lock();
	m_mapConnectList.erase(pSkContext->m_socket);
	m_lckConnectList.unlock();

	if (IOCPModule::Instance()->DisconnectEx(pIO))
	{
		//投递失败，直接回收资源
		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
		m_rscSocketContext.put(pSkContext);

		pIO->Reset();
		m_rscIoContext.put(pIO);
	}
}

void IOCPBase::DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pIO->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
	if (iResult)
	{
		MLOG("Server断开连接失败，错误码:%d", iResult);
		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
	}

	m_rscSocketContext.put(pSkContext);

	pIO->Reset();
	m_rscIoContext.put(pIO);
}

void IOCPBase::PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pIO->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&m_aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			MLOG("检测到连接断开，故停止接收数据！");
			return;
		}
	}

	if (IOCPModule::Instance()->Receive(pIO))	//接受失败的处理
	{
		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
}

void IOCPBase::DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	MLOG("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("网络的另一端已下线！");
		else
			MLOG("SRV接收失败，错误码：%d", iResult);

		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
	else if (0 == dwBytesTransfered)	//代表服务器已断开此socket连接
	{
		MLOG("网络的另一端已断开socket：%d的连接！", pIO->m_socket);

		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		UnpackReceivedData(pSkContext, pIO);
	}

}

void IOCPBase::UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	pIO->m_uDataLength += pIO->m_overlapped.InternalHigh;
	char *buf = pIO->m_szBuffer;

	PackHeader *head = NULL;
	unsigned uDataLength = 0;
	unsigned uPackLength = 0;

	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->m_uDataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_RCV_BUF_LEN)	//接收到的数据不正确
		{
			MLOG("接收到的数据长度太大，断开连接！");
			PostDisconnectEx(pSkContext, pIO);
			break;
		}
		else if (uDataLength < sizeof(PackHeader) || uDataLength < uPackLength)	//不够 一个包头或者一包数据
		{
			if (buf != pIO->m_szBuffer)
			{
				memmove(pIO->m_szBuffer, buf, uDataLength);
			}
			pIO->m_wsaBuf.buf = pIO->m_szBuffer + uDataLength;
			pIO->m_wsaBuf.len = pIO->m_uBufLength - uDataLength;
			PostReceive(pSkContext, pIO);
			break;
		}
		else //至少有一个完整的网络包，开始拆包
		{
			if (CHECK_VALUE != head->uCheck)
			{
				MLOG("包校验不对，将断开连接！");
				PostDisconnectEx(pSkContext, pIO);
				break;
			}
			m_pNetInterface->HandSrvData(pIO->m_socket, head->uMsgType, buf, uPackLength);	//调用具体的业务处理数据，有包头和包体
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}
