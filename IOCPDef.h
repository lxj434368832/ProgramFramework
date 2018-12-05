#pragma once
/*************************************************************************
* function：iocp公共定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"
#include <queue>

#define MAX_BUF_LEN 512 //512B
#define RECEIVE_BUF_COUNT 10


// 在完成端口上投递的I/O操作的类型
enum EOperateType
{
	EOP_UNKNOWN,
	EOP_CONNECT,
	EOP_ACCEPT,
	EOP_RECEIVE,
	EOP_SEND,
	EOP_DISCONNECT,
	EOP_KEEP_LIVE
};

#pragma pack(push,1)
struct PackHeader
{
	unsigned long ulBodyLength;
	int			  msgType;
};
#pragma pack(pop)


//单IO数据结构体定义(用于每一个重叠操作的参数)
struct PER_IO_CONTEXT
{
	OVERLAPPED	m_overlapped;
	SOCKET		m_socket;
	EOperateType m_oprateType;
	WSABUF		m_wsaBuf;
	char		*m_szBuffer;
	unsigned	m_uBufLength;
	unsigned	m_uDataLength;
	WPARAM		m_wParam;			// 扩展参数
	LPARAM		m_lParam;			// 扩展参数2

	PER_IO_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		m_uBufLength = MAX_BUF_LEN;
		m_szBuffer = new char[MAX_BUF_LEN];

		reset();
	}

	virtual ~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		RELEASE_SOCKET(m_socket);		//释放socket资源，可能不必要
	}
	void reset()
	{
		ZeroMemory(&m_overlapped, sizeof(m_overlapped));
		ZeroMemory(m_szBuffer, MAX_BUF_LEN);
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = m_uBufLength;
		m_uDataLength = 0;
		m_wParam = 0;
		m_lParam = 0;
	}
};

struct PER_RECEIVE_IO_CONTEXT :public PER_IO_CONTEXT
{
	PER_RECEIVE_IO_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		m_uBufLength = MAX_BUF_LEN * RECEIVE_BUF_COUNT;
		m_szBuffer = new char[m_uBufLength];

		reset();
	}
};

//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================

struct PER_SOCKET_CONTEXT
{
	SOCKET					m_Socket;                   // 每一个客户端连接的Socket
	SOCKADDR_IN				m_ClientAddr;               // 客户端的地址
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//接收上下文
	int						m_iDisconnectFlag;			//断开连接标识
	int						m_iSendPendingFlag;			//发送请求标识
	std::queue<PER_IO_CONTEXT*> m_queueIoContext;       //客户端发送数据请求的上下队列

	// 初始化
	PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
	}

	// 释放资源
	~PER_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
	}
};
