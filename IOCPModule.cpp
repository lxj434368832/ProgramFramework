#include "stdafx.h"
#include "IOCPModule.h"
#include "IOCPDef.h"
#include <mstcpip.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Kernel32.lib")

IOCPModule::IOCPModule()
{
	WSADATA wsaData;
	int nResult;
	nResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

	char Assert[1];
	NO_ERROR != nResult && Assert[-0xffff];
	(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) && Assert[-0xffff];

	LoadAllWSAFunction();
}

IOCPModule::~IOCPModule()
{
	::WSACleanup();
}

IOCPModule* IOCPModule::Instance()
{
	static IOCPModule s_instance;
	return &s_instance;
}

SOCKET IOCPModule::Socket()
{
	return::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

int IOCPModule::Bind(SOCKET s, const LPSOCKADDR name)
{
	int iRet = 0;
	if (SOCKET_ERROR == ::bind(s, name, sizeof(SOCKADDR)))
	{
		iRet = WSAGetLastError();
		MLOG("bind socket:%d failed, error code:%d ", s, iRet);	//失败可能是端口被占用
	}
	return iRet;
}

int IOCPModule::Listen(SOCKET s, int iMaxUserCount)
{
	int iRet = 0;
	if (SOCKET_ERROR == ::listen(s, iMaxUserCount))
	{
		iRet = WSAGetLastError();
		MLOG("监听socket:%d失败，错误码:%d", s, iRet);
	}
	return iRet;
}

HANDLE IOCPModule::CreateIoCompletionPort()
{
	return::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

int IOCPModule::BindIoCompletionPort(PER_SOCKET_CONTEXT *pSkContext, HANDLE hIOCP)
{
	int iRet = 0;
	if (NULL == ::CreateIoCompletionPort((HANDLE)pSkContext->m_Socket, hIOCP, (DWORD_PTR)pSkContext, 0))
	{
		iRet = ::WSAGetLastError();
		MLOG("socket:%d绑定到完成端口失败，错误码：%d", pSkContext->m_Socket, iRet);
	}
	return iRet;
}

int IOCPModule::AcceptEx(SOCKET listenSocket, PER_IO_CONTEXT *pIO)
{
	int iRet = 0;
	int iSockaddrSize = sizeof(SOCKADDR) + 16;
	DWORD dwBytes = 0;
	pIO->m_oprateType = EOP_ACCEPT;
	if (false == m_fnAcceptEx(listenSocket,pIO->m_socket,pIO->m_wsaBuf.buf,pIO->m_wsaBuf.len - iSockaddrSize*2,
		iSockaddrSize,iSockaddrSize,&dwBytes,&pIO->m_overlapped))
	{
		iRet = ::WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
		{
			MLOG("开启AcceptEx失败，错误码:%d", iRet);
		}
		else
			iRet = 0;
	}
	return iRet;
}

void IOCPModule::GetAcceptExSockaddrs(PER_IO_CONTEXT *pIO, LPSOCKADDR *client)
{
	LPSOCKADDR local = NULL;
	int iAddrLen = sizeof(SOCKADDR);
	DWORD dwFlag = 0;
	int iRet = 0;
	m_fnGetAcceptExSockaddrs(pIO->m_wsaBuf.buf, pIO->m_wsaBuf.len - (iAddrLen + 16) * 2, iAddrLen, iAddrLen,
		&local, &iAddrLen, client, &iAddrLen);
}

int IOCPModule::ConnectEx(PER_IO_CONTEXT *pIO, const LPSOCKADDR name)
{
	int bRet = 0;
	DWORD dwLen = 0;
	if (false == m_fnConnectEx(pIO->m_socket, name, sizeof(SOCKADDR),
			pIO->m_wsaBuf.buf, pIO->m_wsaBuf.len, &dwLen,&pIO->m_overlapped))
	{
		bRet = GetLastError();
		if (WSA_IO_PENDING != bRet)
		{
			MLOG("投递ConnectEx失败，错误码：%d", bRet);
		}
		else
			bRet = 0;

	}
	return bRet;
}

int IOCPModule::DisconnectEx(PER_IO_CONTEXT *pIO)
{
	int iRet = 0;
	if (false == m_fnDisconnectEx(pIO->m_socket,&pIO->m_overlapped, TF_REUSE_SOCKET,0))
	{
		iRet = GetLastError();
		if (WSA_IO_PENDING != iRet)
		{
			MLOG("调用失败，错误码：%d", iRet);
		}
		else
		{
			iRet = 0;
		}
	}
	return iRet;
}

int IOCPModule::PostQueuedCompletionStatus(HANDLE hCP, DWORD dwTransBytes, ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOL)
{
	int iRet = 0;
	if (false == ::PostQueuedCompletionStatus(hCP, dwTransBytes, dwCompletionKey, lpOL))
	{
		iRet = ::GetLastError();
		MLOG("调用失败，错误码：%d", iRet);
	}
	return iRet;
}

int IOCPModule::Send(PER_IO_CONTEXT *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;

	if (SOCKET_ERROR == WSASend(pIO->m_socket, &pIO->m_wsaBuf, 1, nullptr, dwFlags, &pIO->m_overlapped, NULL))
	{
		iRet = WSAGetLastError();
		if (iRet != WSA_IO_PENDING)
			MLOG("发送数据失败，错误码：%d", iRet);
		else
			iRet = 0;

	}
	return iRet;
}

int IOCPModule::Receive(PER_IO_CONTEXT *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;
	if (SOCKET_ERROR == WSARecv(pIO->m_socket, &pIO->m_wsaBuf, 1, nullptr, &dwFlags, &pIO->m_overlapped, nullptr))
	{
		iRet = WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
			MLOG("接收失败，错误码：%d", iRet);
		else
			iRet = 0;
	}
	return iRet;
}

bool IOCPModule::SetKeepLiveParam(PER_IO_CONTEXT *pIO)
{
	bool bKeepAlive = true;
	int nRet = setsockopt(pIO->m_socket, SOL_SOCKET, SO_KEEPALIVE,
		(char*)&bKeepAlive, sizeof(bKeepAlive));
	if (nRet == SOCKET_ERROR)
	{
		MLOG("setsockopt failed: %d/n", WSAGetLastError());
		return false;
	}

	// set KeepAlive parameter
	tcp_keepalive alive_in;
	alive_in.keepalivetime = 5000;  // 5s
	alive_in.keepaliveinterval = 1000; //1s,total time 1000*10(10s)
	alive_in.onoff = TRUE;
	unsigned long ulBytesReturn = 0;

	nRet = WSAIoctl(pIO->m_socket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		nullptr, 0, &ulBytesReturn, &pIO->m_overlapped, NULL);
	if (nRet == SOCKET_ERROR)
	{
		MLOG("WSAIoctl failed: %d/n", WSAGetLastError());
		return false;
	}
}

void IOCPModule::LoadAllWSAFunction()
{
	DWORD dwBytes = 0;
	GUID guid;
	char Assert[1];

	SOCKET socket = Socket();
	if (INVALID_SOCKET == socket)
	{
		MLOG( "创建socket失败,错误码是：%d" ,WSAGetLastError());
		Assert[-0xffff];
	}

	guid = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnConnectEx, sizeof(m_fnConnectEx), &dwBytes, NULL, NULL))
	{
		MLOG("获取函数指针失败，错误码：%d", WSAGetLastError());
	}

	guid = WSAID_ACCEPTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnAcceptEx, sizeof(m_fnAcceptEx), &dwBytes, NULL, NULL))
	{
		MLOG("获取函数指针失败，错误码：%d", WSAGetLastError());
	}

	guid = WSAID_DISCONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnDisconnectEx, sizeof(m_fnDisconnectEx), &dwBytes, NULL, NULL))
	{
		MLOG("获取函数指针失败，错误码：%d", WSAGetLastError());
	}

	guid = WSAID_GETACCEPTEXSOCKADDRS;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnGetAcceptExSockaddrs, sizeof(m_fnGetAcceptExSockaddrs), &dwBytes, NULL, NULL))
	{
		MLOG("获取函数指针失败，错误码：%d", WSAGetLastError());
	}

	closesocket(socket);
}

