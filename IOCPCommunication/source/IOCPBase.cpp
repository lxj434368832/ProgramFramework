#include "..\include\IOCPDef.h"
#include "..\include\IOCPBase.h"
#include "..\include\IOCPModule.h"
#include "..\include\stdafx.h"
#include <assert.h>

IOCPBase::IOCPBase(INetInterface *pNet)
:d(new IOCPBaseData(pNet))
{
	if (d->pNetInterface)
	{
		d->pNetInterface->SetNetworkCallback(std::bind(&IOCPBase::Send, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			std::bind(&IOCPBase::Disconnect, this, std::placeholders::_1));
	}
}

IOCPBase::~IOCPBase()
{
	RELEASE(d);
}

bool IOCPBase::InitIOCP(unsigned uThreadCount)
{
	d->hIOCompletionPort = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == d->hIOCompletionPort)
	{
		LOGM("创建完成端口失败，错误码：%d", GetLastError());
		return false;
	}

	if (0 == uThreadCount)
		d->uThreadCount = IOCPModule::Instance()->GetProcessorCount() * 2 + 1;
	else
		d->uThreadCount = uThreadCount;

	d->aThreadList = new std::thread[d->uThreadCount];
	for (unsigned i = 0; i < d->uThreadCount; i++)
	{
		d->aThreadList[i] = std::move(std::thread(&IOCPBase::WorkThread, this));
	}

	return true;
}

void IOCPBase::UninitIOCP()
{
	logm() << "优雅地关闭完成端口线程队列。";
	//优雅地关闭线程
	for (unsigned i = 0; i < d->uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(d->hIOCompletionPort, 0, NULL, NULL);
	}

	for (unsigned i = 0; i < d->uThreadCount; i++)
	{
		if (d->aThreadList[i].joinable())
		{
			d->aThreadList[i].join();
		}
	}
	delete[] d->aThreadList;
	d->aThreadList = nullptr;

	logm() << "关闭完成端口。";
	//关闭完成端口
	RELEASE_HANDLE(d->hIOCompletionPort);
}

bool IOCPBase::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	/*do
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
				LOGM("创建Server监听socket失败，错误码：%d", WSAGetLastError());
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
	}*/
	return bRet;
}

bool IOCPBase::AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	/*PER_SOCKET_CONTEXT *pSkContext = m_rscSocketContext.get();
	do
	{
		PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
		pIO->m_lParam = iRecnnt;

		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, m_hIOCompletionPort)) break;
		}


		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if(IOCPModule::Instance()->Bind(pSkContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

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
	}
	else
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		m_rscSocketContext.put(pSkContext);
	}*/
	return bRet;
}

void IOCPBase::WorkThread()
{
	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	LOGM("线程:%d开始运行。", std::this_thread::get_id());

	while (true)
	{
		int iResult = 0;
		DWORD dwBytesTransfered = 0;
		LPOVERLAPPED pOverlapped = NULL;
		BOOL bSuccess = ::GetQueuedCompletionStatus(d->hIOCompletionPort,
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
			if (WAIT_TIMEOUT == iResult)
			{
				::Sleep(1000);
				continue;
			}
			else if (0 == iResult)
			{
				logm() << "ThreadID:" << ::GetCurrentThreadId() << " 正常退出！";
			}
			else if (ERROR_ABANDONED_WAIT_0 == iResult)//735L
				LOGM("完成端口句柄:%d被人为地关闭了!", d->hIOCompletionPort);
			else
				LOGM("完成端口发生了错误，错误码:%d ", iResult);

			break;
		}

		if (nullptr == pSocketContext)
		{
			LOGM("线程:%d正常退出！", ::GetCurrentThreadId());
			break;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		HandServerOperate(iResult, pSocketContext, pIO, dwBytesTransfered);

	}

}

void IOCPBase::HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
		HandAccept(iResult, pSkContext, pIO);
		break;
	case EOP_CONNECT:
		HandConnect(iResult, pSkContext, pIO);
		break;

	case EOP_DISCONNECT:
		HandDisconnect(iResult, pSkContext, pIO);
		break;

	case EOP_RECEIVE:
		HandReceive(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	case EOP_SEND:
		HandSend(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	default:
		LOGM("未知的服务操作码：%d", pIO->m_oprateType);
		assert(false);
		break;
	}
}

bool IOCPBase::PostConnectEx(PER_SOCKET_CONTEXT *pSkContext)
{
	//if (0 < ::InterlockedExchangeAdd(&pSkContext->m_iDisconnectFlag, 0))
	//{
	//	LOGM("检测到连接已经断开，将直接回收资源！");
	//	//直接回收资源
	//	d->lckConnectList.lock();
	//	d->mapConnectList.erase(pSkContext->m_uUserKey);
	//	d->lckConnectList.unlock();
	//	RELEASE_SOCKET(pSkContext->m_socket);
	//	d->rscSocketContext.put(pSkContext);
	//	return false;
	//}

	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	pIO->m_socket = pSkContext->m_socket;
	pIO->m_oprateType = EOP_CONNECT;
	if (IOCPModule::Instance()->ConnectEx(pIO, (LPSOCKADDR)&pSkContext->m_clientAddr))
	{
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, false);
		pIO->Reset();
		d->rscIoContext.put(pIO);
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return false;
	}
	else
	{
		d->lckConnectList.lock();
		d->mapConnectList[pSkContext->m_uUserKey] = pSkContext;
		d->lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::HandConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (0 == iResult)
	{
		//通知上层连接成功
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, true);
		//投递接收
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pSkContext, pIO);
		LOGM("UserKey:%d socket:%d连接成功！", pSkContext->m_uUserKey, pSkContext->m_socket);
	}
	else
	{
		if (ERROR_OPERATION_ABORTED == iResult || WSAENOTSOCK == iResult)
		{
			logm() << "用户取消连接。";
		}
		else if (ERROR_CONNECTION_REFUSED == iResult)
		{
			logm() << "服务端还未开启，将会重试。";
		}
		else
		{
			LOGM("socket:%d投递ConnectEx失败，错误码：%d。", pSkContext->m_socket, iResult);
		}

		//通知上层连接失败
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, false);
		d->lckConnectList.lock();
		d->mapConnectList.erase(pSkContext->m_uUserKey);
		d->lckConnectList.unlock();
		d->rscSocketContext.put(pSkContext);
	}
}

bool IOCPBase::PostAcceptEx(SOCKET listenSocket)
{
	if (INVALID_SOCKET == listenSocket)
	{
		LOGM("listenSocket无效！");
		return false;
	}

	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();
	pSkContext->m_uUserKey = d->uUserNum++;
	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	pIO->m_uUserKey = pSkContext->m_uUserKey;

	bool bRet = false;
	for (int i = 0; i < 5; i++)
	{
		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("创建socket失败，错误码：%d", WSAGetLastError());
				return false;
			}
			IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort);
		}
	
		pIO->m_socket = pSkContext->m_socket;
		if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//失败
		{
			RELEASE_SOCKET(pSkContext->m_socket);
		}
		else
		{
			d->lckConnectList.lock();
			d->mapConnectList[pSkContext->m_uUserKey] = pSkContext;
			d->lckConnectList.unlock();
			bRet = true;
			LOGM("socket:%d userKey:%d 投递AcceptEx成功。", pSkContext->m_socket, pSkContext->m_uUserKey);
			break;
		}
	}

	if (false == bRet)
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		LOGM("userKey:%d投递AcceptEx失败！", pSkContext->m_uUserKey);
	}
	return bRet;
}

void IOCPBase::HandAccept(int iResult, PER_SOCKET_CONTEXT *pListenSkContext, PER_IO_CONTEXT* pIO)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	d->lckConnectList.lock();
	if (d->mapConnectList.find(pIO->m_uUserKey) != d->mapConnectList.end())	//服务端的userkey就是socket
	{
		pSkContext = d->mapConnectList[pIO->m_uUserKey];
	}
	d->lckConnectList.unlock();

	if (0 == iResult)	//投递AcceptEx成功
	{
		SOCKADDR_IN *pClientAddr = nullptr;
		IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pClientAddr);

		LOGM("客户端%s:%d连入,用户套接字：%d", IOCPModule::Instance()->GetIPAddress(&pClientAddr->sin_addr).c_str(), 
			ntohs(pClientAddr->sin_port), pSkContext->m_socket);

		if (pSkContext)
		{
			memcpy(&(pSkContext->m_clientAddr), pClientAddr, sizeof(SOCKADDR_IN));
		}
		else
		{
			LOGM("pClientSkContext为空，发生错误！");
			assert(pSkContext);
		}

		//通知服务端新连接到达
		d->pNetInterface->AddUser(pSkContext->m_uUserKey);
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pSkContext, pIO);
		PostAcceptEx(pListenSkContext->m_socket);
	}
	else
	{
		LOGM("投递AcceptEx失败，错误码：%d", iResult);
		if (pSkContext)
		{
			d->lckConnectList.lock();
			d->mapConnectList.erase(pSkContext->m_uUserKey);
			d->lckConnectList.unlock();

			int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
			d->aLckSocketContext[idxLock].lock();
			RELEASE_SOCKET(pSkContext->m_socket);
			d->aLckSocketContext[idxLock].unlock();

			d->rscSocketContext.put(pSkContext);
		}
	}
}

void IOCPBase::Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	if (nullptr == data || 0 == uLength)
	{
		LOGM("发送的数据为空！");
		return;
	}

	if (uLength > MAX_RCV_BUF_LEN - sizeof(PackHeader))
	{
		LOGM("需要发送数据(%d)大于发送缓冲区(%d),不能发送数据！", uLength, MAX_RCV_BUF_LEN - sizeof(PackHeader));
		return;
	}

	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	int idxLock = uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;

	d->lckConnectList.lock();
	if (d->mapConnectList.find(uUserKey) != d->mapConnectList.end())
	{
		pSkContext = d->mapConnectList.at(uUserKey);
	}
	d->lckConnectList.unlock();

	if (nullptr == pSkContext)
	{
		LOGM("发送数据失败，不存在当前用户:%d", uUserKey);
		return;
	}

	d->aLckSocketContext[idxLock].lock();
	PackSendData(pSkContext, uMsgType, data, uLength);
	d->aLckSocketContext[idxLock].unlock();

	PostSend(pSkContext);
	return;
}

void IOCPBase::PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length)
{
	unsigned uLeftLen = length;
	unsigned uCurrentCpyLen = 0;

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		LOGM("当前用户:%d已断开，停止发送数据", pSkContext->m_socket);
		return;
	}

	for (int i = 0; 0 != uLeftLen; i++)
	{
		if (uLeftLen < 0)
		{
			LOGM("发送数据逻辑出现错误，uLeftLen小于0！");
			break;
		}

		PER_IO_CONTEXT *pIO = d->rscIoContext.get();
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
		//将当前IO放入发送队列
		pSkContext->m_queueIoContext.push(pIO);

		uLeftLen -= uCurrentCpyLen;
		data += uCurrentCpyLen;
	}

}

bool IOCPBase::PostSend(PER_SOCKET_CONTEXT *pSkContext)
{
	bool bRet = true;
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;

	d->aLckSocketContext[idxLock].lock();
	if (pSkContext->m_queueIoContext.empty())
		d->aLckSocketContext[idxLock].unlock();
	else
	{
		PER_IO_CONTEXT *pIO = pSkContext->m_queueIoContext.front();
		pSkContext->m_queueIoContext.pop();
		d->aLckSocketContext[idxLock].unlock();

		if (IOCPModule::Instance()->Send(pIO))	//发送失败的处理
		{
			pIO->Reset();
			PostDisconnectEx(pSkContext, pIO);
			bRet = false;
		}
	}
	return bRet;
}

void IOCPBase::HandSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	LOGM("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			LOGM("网络的另一端已下线！");
		else
			LOGM("发送数据失败，错误码：%d", iResult);
		pIO->Reset();
		PostDisconnectEx(pSkContext, pIO);
	}
	else if (0 == dwBytesTransfered)	//代表服务器已断开此socket连接
	{
		LOGM("网络的另一端已断开socket：%d的连接！", pSkContext->m_socket);
		pIO->Reset();
		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		pIO->Reset();
		d->rscIoContext.put(pIO);

		PostSend(pSkContext);
	}
}

void IOCPBase::Disconnect(unsigned uUserKey)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	PER_IO_CONTEXT *pIO = nullptr;

	d->lckConnectList.lock();
	if (d->mapConnectList.end() != d->mapConnectList.find(uUserKey))
	{
		pSkContext = d->mapConnectList.at(uUserKey);
	}
	d->lckConnectList.unlock();

	if (pSkContext)
	{
		PostDisconnectEx(pSkContext, d->rscIoContext.get());
	}
}

void IOCPBase::PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;

	ReCycleSocketRsc(pSkContext, pIO);

	if (1 != pSkContext->m_iDisconnectFlag) return;

	//从连接列表中清除
	d->lckConnectList.lock();
	d->mapConnectList.erase(pSkContext->m_uUserKey);
	d->lckConnectList.unlock();

	pIO->m_socket = pSkContext->m_socket;
	if (IOCPModule::Instance()->DisconnectEx(pIO))
	{
		//投递失败，直接回收资源
		d->aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		d->aLckSocketContext[idxLock].unlock();
		d->rscSocketContext.put(pSkContext);

		pIO->Reset();
		d->rscIoContext.put(pIO);
	}
}

void IOCPBase::ReCycleSocketRsc(PER_SOCKET_CONTEXT * pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;

	if (0 == pSkContext->m_iDisconnectFlag)
	{
		pSkContext->m_iDisconnectFlag = 1;
		//清理发送队列
		d->aLckSocketContext[idxLock].lock();
		while (false == pSkContext->m_queueIoContext.empty())
		{
			PER_IO_CONTEXT *pIOTemp = pSkContext->m_queueIoContext.front();
			pIOTemp->Reset();
			d->rscIoContext.put(pIOTemp);
			pSkContext->m_queueIoContext.pop();
		}
		d->aLckSocketContext[idxLock].unlock();

		//通知上层接口关闭连接
		d->pNetInterface->DeleteUser(pSkContext->m_uUserKey);
		LOGM("关闭user:%d的连接。", pSkContext->m_uUserKey);
	}
	else if (1 == pSkContext->m_iDisconnectFlag)
	{
		//已经处理过断开连接了,回收IO资源
		pSkContext->m_iDisconnectFlag = 2;
		if (pIO)
		{
			pIO->Reset();
			d->rscIoContext.put(pIO);
		}
		LOGM("user:%d已经断开连接，将不处理。", pSkContext->m_uUserKey);
	}
}

void IOCPBase::HandDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
		{
			LOGM("断开连接失败，网络的另一端已下线！");
		}
		else
		{
			LOGM("断开连接失败，错误码:%d", iResult);
		}
		d->aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		d->aLckSocketContext[idxLock].unlock();
	}
	else
		LOGM("socket:%d将会重用。", pSkContext->m_socket);

	d->rscSocketContext.put(pSkContext);

	pIO->Reset();
	d->rscIoContext.put(pIO);
}

void IOCPBase::PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (0 < pSkContext->m_iDisconnectFlag)
	{
		ReCycleSocketRsc(pSkContext, nullptr);
		LOGM("检测到连接断开，故停止接收数据！");
		return;
	}

	if (IOCPModule::Instance()->Receive(pIO))	//接收失败的处理
	{
		PostDisconnectEx(pSkContext, d->rscIoContext.get());
	}
}

void IOCPBase::HandReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	LOGM("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			LOGM("网络的另一端已下线！");
		else if (ERROR_OPERATION_ABORTED == iResult)
			LOGM("用户已取消socket：%d的连接！", pSkContext->m_socket);
		else
			LOGM("SRV接收失败，错误码：%d", iResult);

		PostDisconnectEx(pSkContext, d->rscIoContext.get());
	}
	else if (0 == dwBytesTransfered)	//代表服务器已断开此socket连接
	{
		LOGM("网络的另一端已断开socket：%d的连接！", pSkContext->m_socket);

		PostDisconnectEx(pSkContext, d->rscIoContext.get());
	}
	else
	{
		UnpackReceivedData(pSkContext, pIO);
	}
}

void IOCPBase::UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	PackHeader *head = NULL;
	char *buf = pIO->m_szBuffer;

	unsigned uDataLength = 0;	//剩余数据的长度
	unsigned uPackLength = 0;	//待拆包数据的长度

	pIO->m_uDataLength += pIO->m_overlapped.InternalHigh;
	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->m_uDataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_RCV_BUF_LEN)	//接收到的数据不正确
		{
			LOGM("接收到的数据长度太大，断开连接！");
			PostDisconnectEx(pSkContext, d->rscIoContext.get());
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
				LOGM("包校验不对，将断开连接！");
				PostDisconnectEx(pSkContext, d->rscIoContext.get());
				break;
			}
			d->pNetInterface->HandData(pSkContext->m_uUserKey, head->uMsgType, buf + sizeof(PackHeader), head->ulBodyLength);	//调用具体的业务处理数据，有包头和包体
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}
