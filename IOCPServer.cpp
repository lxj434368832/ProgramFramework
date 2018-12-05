#include "stdafx.h"
#include "IOCPServer.h"
#include "IOCPModule.h"
#include "ConnectManage.h"
#include "ServerManage.h"
#include "ClientManage.h"
#include <WS2tcpip.h>
#include <assert.h>

IOCPServier::IOCPServier(ServerManage *pSrvMng) :
m_pSrvMng(pSrvMng),
m_hIOCompletionPort(INVALID_HANDLE_VALUE),
m_rscSocketContext(10000),
m_rscIoContext(1000)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;
	m_pListenSocketContext = nullptr;
}

IOCPServier::~IOCPServier()
{
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCompletionPort, 0, NULL, NULL);
	}

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::WaitForSingleObject(m_aThreadList[i], INFINITE);
		::CloseHandle(m_aThreadList[i]);
	}
	delete[] m_aThreadList;

	RELEASE_HANDLE(m_hIOCompletionPort);
	RELEASE(m_pListenSocketContext);

}

bool IOCPServier::InitIOCP(unsigned uThreadCount)
{
	assert(uThreadCount);

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

bool IOCPServier::StartServerListen(u_short port, unsigned iMaxServerCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = m_rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_Socket)
		{
			m_pListenSocketContext->m_Socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_Socket)
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
		if (IOCPModule::Instance()->Bind(m_pListenSocketContext->m_Socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(m_pListenSocketContext->m_Socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, m_hIOCompletionPort)) break;

		//投递接受操作
		for (unsigned i = 0; i < iMaxServerCount; i++)
		{
			if (false == (bRet = PostAcceptEx(m_pListenSocketContext->m_Socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		//::closesocket(m_pListenSocketContext->m_Socket);

		//m_pSrvMng->SetSrvListenSocket(lstnSocket);
	}
	return bRet;
}

void IOCPServier::Send(int key, std::string data)
{

}

DWORD WINAPI  IOCPServier::WorkerThread(LPVOID lpParameter)
{
	int iResult = 0;
	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	IOCPServier *pThis = (IOCPServier*)lpParameter;
	MLOG("线程:%d开始运行。", ::GetCurrentThreadId());

	while (true)
	{
		int iRet = 0;
		DWORD dwBytesTransfered = 0;
		LPOVERLAPPED pOverlapped = NULL;
		bool bSuccess = ::GetQueuedCompletionStatus(pThis->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITY);

		if (false == bSuccess)
		{
			iResult = ::GetLastError();

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

			pThis->HandleError(pSocketContext, iResult);
			continue;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		// 判断是否有客户端断开了
		if ((0 == dwBytesTransfered) && (EOP_RECEIVE == pIO->m_oprateType || EOP_SEND == pIO->m_oprateType))
		{
			MLOG("客户端 %s:%d 断开连接。", inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));

			// 释放掉对应的资源
			//pIOCPModel->_RemoveContext(pSocketContext);

			continue;
		}
		pThis->HandServerOperate(iResult, pIO);

	}

	return 0;
}

void IOCPServier::HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr)
{
	// 可能是客户端异常退出了
	if (ERROR_NETNAME_DELETED == dwErr)
	{
		MLOG("检测到客户端异常退出！");
	}
	//this->_RemoveContext(pContext);
}

void IOCPServier::HandServerOperate(int iResult, PER_IO_CONTEXT* pIO)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
		if (0 == iResult)	//投递AcceptEx成功
		{
			//pIO->m_uUserId = m_pSrvMng->GetSrvUserId();
			//m_pSrvMng->SrvAcceptNotify(pIO->m_uUserId, pIO->m_socket);
			pIO->m_oprateType = EOP_RECEIVE;
			UnpackReceivedData(pIO, std::bind(&ServerManage::HandSrvData, m_pSrvMng,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

			SOCKADDR_IN *pUserAddr = NULL;
			IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pUserAddr);
			char strIP[255];	inet_ntop(AF_INET, &pUserAddr->sin_addr, strIP, 255);
			MLOG("新用户连入。用户IP：%s，用户套接字：%d", strIP, pIO->m_socket);

			PostAcceptEx(m_pSrvMng->GetSrvListenSocket());	//
		}
		else
		{
			MLOG("投递AcceptEx失败，错误码：%d", iResult);
			//ReleaseIOContext(pIO);			//？这个有待验证
		}
		break;

	case EOP_DISCONNECT:
		if (iResult)
		{
			MLOG("Server断开连接失败，错误码:%d", iResult);
		}
		//ReleaseIOContext(pIO);
		break;

	case EOP_RECEIVE:
		DoReceive(pIO, iResult, std::bind(&ServerManage::HandSrvData, m_pSrvMng,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		break;

	case EOP_SEND:
		if (0 == pIO->m_overlapped.InternalHigh)
		{
			MLOG("网络的另一端已断开socket：%d 的连接！");
			shutdown(pIO->m_socket, SD_BOTH);
		}
		else if (iResult)
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("网络的另一端已下线！");
			else
				MLOG("SRV发送失败，错误码：%d", iResult);

			shutdown(pIO->m_socket, SD_BOTH);
		}

		//ReleaseIOContext(pIO);

		break;
	default:
		MLOG("未知的服务操作码：%d", pIO->m_oprateType);
		MAssert(false);
		break;
	}
}

void IOCPServier::DoReceive(PER_IO_CONTEXT* pIO, int iResult, std::function<void(unsigned, const char*, unsigned)> HandData)
{
	if (0 == pIO->m_overlapped.InternalHigh)	//代表服务器已断开此socket连接
	{
		MLOG("网络的另一端已断开socket：%d 的连接！错误码：%d", pIO->m_socket, iResult);
		PostDisconnectEx(pIO);
	}
	else if (0 == iResult)
	{
		UnpackReceivedData(pIO, HandData);
	}
	else
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("网络的另一端已下线！");
		else
			MLOG("SRV接收失败，错误码：%d", iResult);

		PostDisconnectEx(pIO);
	}
}

//
void IOCPServier::UnpackReceivedData(PER_IO_CONTEXT* pIO, std::function<void(unsigned, const char*, unsigned)> HandData)
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

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_BUF_LEN)	//接收到的数据不正确
		{
			MLOG("接收到的数据长度太大，断开连接！");
			PostDisconnectEx(pIO);
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
			PostReceive(pIO);
			break;
		}
		else //至少有一个完整的网络包，开始拆包
		{
			//HandData(pIO->m_uUserId, buf, uPackLength);	//调用具体的业务处理数据，有包头和包体
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}

bool IOCPServier::PostAcceptEx(SOCKET listenSocket)
{
	if (INVALID_SOCKET == listenSocket)
	{
		MLOG("listenSocket无效！");
		return false;
	}

	PER_SOCKET_CONTEXT *pSocketContext = m_rscSocketContext.get();

	if (INVALID_SOCKET == pSocketContext->m_Socket)
	{
		if (INVALID_SOCKET == (pSocketContext->m_Socket = IOCPModule::Instance()->Socket()))
		{
			MLOG("创建socket失败，错误码：%d", WSAGetLastError());
			return false;
		}
		MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pSocketContext, m_hIOCompletionPort));
	}
	PER_IO_CONTEXT *pIO = &pSocketContext->m_ReceiveContext;
	pIO->m_socket = pSocketContext->m_Socket;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//失败
	{
		m_rscSocketContext.put(pSocketContext);
		return false;
	}
	else
	{
		m_mapConnectList[pSocketContext->m_Socket] = pSocketContext;
		return true;
	}
}

void IOCPServier::PostDisconnectEx(PER_IO_CONTEXT* pIO)
{
	UserInfo *pInfo = NULL;

	//pInfo = m_pCnntMng->DeleteCnntUser(pIO->m_uUserId);

	if (pInfo)
	{
		pIO->m_oprateType = EOP_DISCONNECT;
		if (IOCPModule::Instance()->DisconnectEx(pIO))
		{
			if (pIO->m_lParam)
			{
				//m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->m_lParam);
			}

			//ReleaseIOContext(pIO);
		}
	}
	else
	{
		MLOG("用户不存在，不应该执行到这里请注意！");

		if (pIO->m_lParam)
		{
			//m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->m_lParam);
		}

		//ReleaseIOContext(pIO);
	}

}

void IOCPServier::PostReceive(PER_IO_CONTEXT* pIO)
{
	if (IOCPModule::Instance()->Receive(pIO))	//接受失败的处理
	{
		MAssert(false);
		shutdown(pIO->m_socket, SD_BOTH);
		PostDisconnectEx(pIO);
	}
}

void IOCPServier::PostSend(PER_IO_CONTEXT* pIO, EOperateType op)
{
	pIO->m_oprateType = op;
	if (IOCPModule::Instance()->Send(pIO))	//发送失败的处理
	{
		MAssert(false);
		shutdown(pIO->m_socket, SD_BOTH);	//接受那边会收到错误
		//ReleaseIOContext(pIO);
	}
}

