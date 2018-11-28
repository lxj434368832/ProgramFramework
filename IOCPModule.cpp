#include "stdafx.h"
#include "IOCPModule.h"
#include "IOCPDef.h"

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

int IOCPModule::BindIoCompletionPort(SOCKET s, HANDLE hIOCP)
{
	int iRet = 0;
	if (NULL == ::CreateIoCompletionPort((HANDLE)s,hIOCP,0, 0))
	{
		iRet = WSAGetLastError();
		MLOG("socket:%d绑定到完成端口失败，错误码：%d", s, iRet);
	}
	return iRet;
}

int IOCPModule::AcceptEx(SOCKET srvSocket, IOContext *pIO)
{
	int iRet = 0;
	int iSockaddrSize = sizeof(SOCKADDR) + 16;
	DWORD dwBytes = 0;
	if (false == m_fnAcceptEx(srvSocket,pIO->socket,pIO->wsabuf.buf,pIO->wsabuf.len - iSockaddrSize*2,
		iSockaddrSize,iSockaddrSize,&dwBytes,&pIO->overlapped))
	{
		iRet = WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
		{
			MLOG("开启AcceptEx失败，错误码:%d", iRet);
		}
		else
			iRet = 0;
	}
	return iRet;
}

void IOCPModule::GetAcceptExSockaddrs(IOContext *pIO, LPSOCKADDR *client)
{
	LPSOCKADDR local = NULL;
	int iAddrLen = sizeof(SOCKADDR);
	DWORD dwFlag = 0;
	int iRet = 0;
	m_fnGetAcceptExSockaddrs(pIO->wsabuf.buf, pIO->wsabuf.len - (iAddrLen + 16) * 2, iAddrLen, iAddrLen,
		&local, &iAddrLen, client, &iAddrLen);
}

int IOCPModule::ConnectEx(IOContext *pIO, const LPSOCKADDR name)
{
	int bRet = 0;
	DWORD dwLen = 0;
	if (false == m_fnConnectEx(pIO->socket, name, sizeof(SOCKADDR),
			pIO->wsabuf.buf, pIO->wsabuf.len, &dwLen,&pIO->overlapped))
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

int IOCPModule::DisconnectEx(IOContext *pIO)
{
	int iRet = 0;
	if (false == m_fnDisconnectEx(pIO->socket,&pIO->overlapped, TF_REUSE_SOCKET,0))
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

int IOCPModule::GetQueuedCompletionStatus(HANDLE hcp, IOContext **ppIOContext)
{
	int iRet = 0;
	DWORD dwBytes = 0;
	LPOVERLAPPED pOL = NULL;
	if (false == ::GetQueuedCompletionStatus(hcp, &dwBytes, 0, &pOL, INFINITY))
	{
		iRet = GetLastError();
	}
	*ppIOContext = CONTAINING_RECORD(pOL, IOContext, overlapped);

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

int IOCPModule::Send(IOContext *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;

	if (SOCKET_ERROR == WSASend(pIO->socket, &pIO->wsabuf, 1, NULL, dwFlags, &pIO->overlapped, NULL))
	{
		iRet = WSAGetLastError();
		if (iRet != WSA_IO_PENDING)
			MLOG("发送数据失败，错误码：%d", iRet);
		else
			iRet = 0;

	}
	return iRet;
}

int IOCPModule::Receive(IOContext *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;
	if (SOCKET_ERROR == WSARecv(pIO->socket, &pIO->wsabuf,1,NULL,&dwFlags,&pIO->overlapped,NULL))
	{
		iRet = WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
			MLOG("接收失败，错误码：%d", iRet);
		else
			iRet = 0;
	}
	return iRet;
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

