#pragma once
/*************************************************************************
* function：iocp模型定义文件，封装相关的网络操作，方便调用系统的网络操作
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"
#include <MSWSock.h>

struct PER_IO_CONTEXT;

// 主要封装iocp相关的连接、接收、发送函数

class IOCPModule
{
public:
	~IOCPModule();
	static IOCPModule* Instance();

	SOCKET Socket();

	/*************************************************************************
	* function：绑定socket到本地地址name
	* return:	成功返回0，失败返回WSAGetLastError()的值
	*************************************************************************/
	int Bind(SOCKET s, const LPSOCKADDR name);

	/*************************************************************************
	* function：开启监听功能
	* return:	成功返回0，失败返回WSAGetLastError()的值
	*************************************************************************/
	int Listen(SOCKET s, int iMaxUserCount);

	/*************************************************************************
	* function：创建完成端口
	* return:	成功返回创建的句柄，失败返回NULL
	*************************************************************************/
	HANDLE CreateIoCompletionPort();

	/*************************************************************************
	* function：将socket绑定到完成端口hIOCP上
	* return:	成功返回0，失败返回WSAGetLastError()的值
	*************************************************************************/
	int BindIoCompletionPort(SOCKET socket, HANDLE hIOCP);
	
	/*************************************************************************
	* function：开启客户端的连接
	* return:	成功返回0，失败返回WSAGetLastError()的值
	*************************************************************************/
	int AcceptEx(SOCKET srvSocket, PER_IO_CONTEXT *pIO);

	void GetAcceptExSockaddrs(PER_IO_CONTEXT *pIO, LPSOCKADDR *client);

	//成功返回0，不成功返回GetLastError()的值
	int ConnectEx(PER_IO_CONTEXT *pIO, const LPSOCKADDR name);

	//成功返回0，不成功返回GetLastError()的值
	int DisconnectEx(PER_IO_CONTEXT *pIO);

	//成功返回0，不成功返回GetLastError()的值
	int GetQueuedCompletionStatus(HANDLE hcp, PER_IO_CONTEXT **ppIOContext);

	//成功返回0，不成功返回GetLastError()的值
	int PostQueuedCompletionStatus(HANDLE hCP, DWORD dwTransBytes, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOL);

	//成功返回0，不成功返回GetLastError()的值
	int Send(PER_IO_CONTEXT *pIO);

	//成功返回0，不成功返回GetLastError()的值
	int Receive(PER_IO_CONTEXT *pIO);

private:
	IOCPModule();
	IOCPModule(const IOCPModule&);
	IOCPModule& operator=(const IOCPModule&);
	void LoadAllWSAFunction();

private:
	LPFN_CONNECTEX m_fnConnectEx = NULL;
	LPFN_ACCEPTEX m_fnAcceptEx = NULL;
	LPFN_DISCONNECTEX m_fnDisconnectEx = NULL;
	LPFN_GETACCEPTEXSOCKADDRS m_fnGetAcceptExSockaddrs = NULL;
};
