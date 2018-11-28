#include "stdafx.h"
#include "IOCPClient.h"
#include "IOCPModule.h"
#include "ConnectManage.h"
#include "ServerManage.h"
#include "ClientManage.h"
#include <WS2tcpip.h>

IOCPClient::IOCPClient(ConnectManage *pCnntMng,
	ServerManage *pSrvMng, ClientManage *pClientMng):
	 m_hIOCP(INVALID_HANDLE_VALUE), m_rscIO(1000)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;

	m_pCnntMng = pCnntMng;
	m_pSrvMng = pSrvMng;
	m_pClientMng = pClientMng;
}

IOCPClient::~IOCPClient()
{
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCP, 0, NULL, NULL);
	}

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::WaitForSingleObject(m_aThreadList[i], INFINITE);
		::CloseHandle(m_aThreadList[i]);
	}

	RELEASE_HANDLE(m_hIOCP);
	delete[] m_aThreadList;
	m_pCnntMng = NULL;
	m_pSrvMng = NULL;
	m_pClientMng = NULL;
}

bool IOCPClient::InitIOCP(unsigned uThreadCount)
{
	assert(uThreadCount);

	m_hIOCP = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == m_hIOCP)
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

DWORD WINAPI  IOCPClient::WorkerThread(LPVOID lpParameter)
{
	IOContext *pIO = NULL;
	int iResult = 0;
	IOCPClient *pThis = (IOCPClient*)lpParameter;
	while (true)
	{
		iResult = IOCPModule::Instance()->GetQueuedCompletionStatus(pThis->m_hIOCP, &pIO);
		if (pIO)
		{
			switch (pIO->oprateType)
			{
			case EOP_CNNT_CONNECT:
			case EOP_CNNT_DISCONNECT:
			case EOP_CNNT_RECEIVE:
			case EOP_CNNT_SEND:
				pThis->HandConnectOperate(iResult, pIO);
				break;

			case EOP_CLIENT_ACCEPT:
			case EOP_CLIENT_DISCONNECT:
			case EOP_CLIENT_RECEIVE:
			case EOP_CLIENT_SEND:
				pThis->HandClientOperate(iResult, pIO);
				break;

			case EOP_SRV_ACCEPT:
			case EOP_SRV_DISCONNECT:
			case EOP_SRV_RECEIVE:
			case EOP_SRV_SEND:
				pThis->HandServerOperate(iResult, pIO);
				break;

			default:
				MLOG("未知的操作类型：%d", pIO->oprateType);
				MAssert(false);
				break;
			}
		}
		else
		{
			MLOG("线程:%d结束！", ::GetCurrentThreadId());
			break;
		}
	}

	return 0;
}

unsigned IOCPClient::StartConnect(std::string ip, u_short port, int iRecnnt = -1, const char* data = nullptr, int len = 0)
{
	unsigned uUserId = 0;
	do 
	{
		IOContext *pIO = m_rscIO.get();
		pIO->wParam = iRecnnt;

		if (INVALID_SOCKET == pIO->socket)
		{
			if (INVALID_SOCKET == (pIO->socket = IOCPModule::Instance()->Socket()))
			{
				MLOG("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
			MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pIO->socket, m_hIOCP));
		}

		SOCKADDR_IN addrLocal;	//貌似这个有问题,bind后被释放掉了
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		MAssert(0 == IOCPModule::Instance()->Bind(pIO->socket, (LPSOCKADDR)&addrLocal));

		if (NULL == m_pCnntMng)
		{
			MLOG("请初始化ConnectManage!");
			break;
		}

		SOCKADDR_IN *pAddrSrv = m_pCnntMng->GetSOCKADDR();
		MAssert(pAddrSrv);
		pAddrSrv->sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &pAddrSrv->sin_addr);
		pAddrSrv->sin_port = htons(port);
		pIO->lParam = (LPARAM)pAddrSrv;

		pIO->uUserId = m_pCnntMng->GetCnntUserId();
		PostConnectEx(pIO, (LPSOCKADDR)pAddrSrv);
		uUserId = pIO->uUserId;		//因为连接失败时pIO->uUserId=0

	} while (0);
	return uUserId;
}

void IOCPClient::HandConnectOperate(int iResult, IOContext* pIO)
{
	switch (pIO->oprateType)
	{
	case EOP_CNNT_CONNECT:
		m_pCnntMng->ConnectNotify(pIO->uUserId, pIO->socket, 0 == iResult);

		if (0 == iResult)
		{
			PostReceive(pIO, EOP_CNNT_RECEIVE);
		}
		else
		{
			if (ERROR_CONNECTION_REFUSED == iResult)	//代表服务端没有启动1225L
				MLOG("连接失败，服务端没有启动!");
			else
				MLOG("连接失败，错误码：%d", iResult);
		
			if ((int)pIO->wParam > 0)
			{
				MLOG("socket:%d开启重连!", pIO->socket);
				Sleep(1000);
				PostConnectEx(pIO, (LPSOCKADDR)pIO->lParam);
			}
			else
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
				ReleaseIOContext(pIO);
			}
		}

		break;
	case EOP_CNNT_DISCONNECT:
		if (0 == iResult)
		{
			if ((int)pIO->wParam > 0)	//需要重连
			{
				MLOG("socket:%d开启重连!", pIO->socket);
				PostConnectEx(pIO, (LPSOCKADDR)pIO->lParam);
			}
			else
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
				ReleaseIOContext(pIO);
			}
		}
		else	//此处不应该进入
		{
			MLOG("断开连接失败，错误码：%d", iResult);
			MAssert(false);
		}

		break;
	case EOP_CNNT_RECEIVE:
		if (0 == pIO->overlapped.InternalHigh)	//代表服务器已断开此socket连接
		{
			MLOG("网络的另一端已断开socket：%d 的连接！",pIO->socket, iResult);
			PostDisconnectEx(pIO, pIO->oprateType);
		}
		else if (0 == iResult)
		{
			UnpackReceivedData(pIO, std::bind(&ConnectManage::HandCnntData, m_pCnntMng,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}
		else
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("网络的另一端已下线！");
			else
				MLOG("connect接收失败，错误码：%d", iResult);

			PostDisconnectEx(pIO, pIO->oprateType);
		}

		break;
	case EOP_CNNT_SEND:
		if (0 == pIO->overlapped.InternalHigh)
		{
			MLOG("网络的另一端已断开socket：%d 的连接！");
			shutdown(pIO->socket, SD_BOTH);
		}
		else if (iResult)
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("网络的另一端已下线！");
			else
				MLOG("connect发送失败，错误码：%d", iResult);

			shutdown(pIO->socket, SD_BOTH);
		}

		ReleaseIOContext(pIO);

		break;
	default:
		MLOG("未知的连接操作码：%d", pIO->oprateType);
		MAssert(false);
		break;
	}
}

void IOCPClient::HandClientOperate(int iResult, IOContext* pIO)
{

}

void IOCPClient::DoReceive(IOContext* pIO, int iResult,std::function<void(unsigned, const char*, unsigned)> HandData)
{
	if (0 == pIO->overlapped.InternalHigh)	//代表服务器已断开此socket连接
	{
		MLOG("网络的另一端已断开socket：%d 的连接！错误码：%d", pIO->socket, iResult);
		PostDisconnectEx(pIO, pIO->oprateType);
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

		PostDisconnectEx(pIO, pIO->oprateType);
	}
}

IOContext* IOCPClient::GetIOContext()
{
	return m_rscIO.get();
}

void IOCPClient::ReleaseIOContext(IOContext *pIO)
{
	pIO->reset();
	m_rscIO.put(pIO);
}

//
void IOCPClient::UnpackReceivedData(IOContext* pIO, std::function<void(unsigned, const char*, unsigned)> HandData)
{
	pIO->dataLength += pIO->overlapped.InternalHigh;
	char *buf = pIO->buffer;

	PackHeader *head = NULL;
	unsigned uDataLength = 0;
	unsigned uPackLength = 0;

	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->dataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_BUF_LEN)	//接收到的数据不正确
		{
			MLOG("接收到的数据长度太大，断开连接！");
			PostDisconnectEx(pIO, pIO->oprateType);
			break;
		}
		else if (uDataLength < sizeof(PackHeader) || uDataLength < uPackLength)	//不够 一个包头或者一包数据
		{
			if (buf != pIO->buffer)
			{
				memmove(pIO->buffer, buf, uDataLength);
			}
			pIO->wsabuf.buf = pIO->buffer + uDataLength;
			pIO->wsabuf.len = pIO->bufLength - uDataLength;
			PostReceive(pIO, pIO->oprateType);
			break;
		}
		else //至少有一个完整的网络包，开始拆包
		{
			HandData(pIO->uUserId, buf, uPackLength);	//调用具体的业务处理数据，有包头和包体
			buf += uPackLength;
			pIO->dataLength -= uPackLength;
		}
	}
}

bool IOCPClient::PostConnectEx(IOContext* pIO, SOCKADDR* pSrvAddr)
{
	MAssert(pIO && pSrvAddr);
	pIO->oprateType = EOP_CNNT_CONNECT;
	if (IOCPModule::Instance()->ConnectEx(pIO, pSrvAddr))
	{
		MAssert(false);
		m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
		ReleaseIOContext(pIO);

		return false;
	}
	return true;
}

bool IOCPClient::PostAcceptEx(SOCKET listenSocket, EOperateType op)
{
	if (INVALID_SOCKET == listenSocket)
	{
		MLOG("listenSocket无效！");
		return false;
	}
	else if (op != EOP_CLIENT_ACCEPT || op != EOP_SRV_ACCEPT)
	{
		MLOG("投递的操作类型错误！");
		return false;
	}

	IOContext *pIO = GetIOContext();
	if (INVALID_SOCKET == pIO->socket)
	{
		if (INVALID_SOCKET == (pIO->socket = IOCPModule::Instance()->Socket()))
		{
			MLOG("创建socket失败，错误码：%d", WSAGetLastError());
			return false;
		}
		MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pIO->socket, m_hIOCP));
	}

	pIO->oprateType = op;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//失败
	{
		ReleaseIOContext(pIO);
		return false;
	}
	else
		return true;
}

void IOCPClient::PostDisconnectEx(IOContext* pIO, EOperateType op)
{
	UserInfo *pInfo = NULL;

	if (op < EOP_CNNT_OPERATE)
	{
		pInfo = m_pCnntMng->DeleteCnntUser(pIO->uUserId);
		op = EOP_CNNT_DISCONNECT;
	}
	else if (op < EOP_CLIENT_OPRATE)
	{
		pInfo = m_pClientMng->DeleteClientUser(pIO->uUserId);
		op = EOP_CLIENT_DISCONNECT;
	}
	else if (op < EOP_SRV_OPERATE)
	{
		pInfo = m_pSrvMng->DeleteSrvUser(pIO->uUserId);
		op = EOP_SRV_DISCONNECT;
	}
	else
	{
		MLOG("未知的操作方法,值为：%d", op);
	}

	if (pInfo)
	{
		pIO->oprateType = op;
		if (IOCPModule::Instance()->DisconnectEx(pIO))
		{
			if (op < EOP_CNNT_OPERATE && pIO->lParam)
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
			}

			ReleaseIOContext(pIO);
		}
	}
	else
	{
		MLOG("用户不存在，不应该执行到这里请注意！");

		if (op < EOP_CNNT_OPERATE && pIO->lParam)
		{
			m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
		}

		ReleaseIOContext(pIO);
	}

}

void IOCPClient::PostReceive(IOContext* pIO, EOperateType op)
{
	pIO->oprateType = op;
	if (IOCPModule::Instance()->Receive(pIO))	//接受失败的处理
	{
		MAssert(false);
		shutdown(pIO->socket, SD_BOTH);
		PostDisconnectEx(pIO, op);
	}
}

void IOCPClient::PostSend(IOContext* pIO, EOperateType op)
{
	pIO->oprateType = op;
	if (IOCPModule::Instance()->Send(pIO))	//发送失败的处理
	{
		MAssert(false);
		shutdown(pIO->socket, SD_BOTH);	//接受那边会收到错误
		ReleaseIOContext(pIO);
	}

}
