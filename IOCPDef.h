#pragma once
/*************************************************************************
* function：iocp公共定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"
#include <queue>

#define MAX_BUF_LEN 512			//发送缓冲区大小512B
#define MAX_RCV_BUF_LEN 5120	//接收缓冲区大小5K

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

		Reset();
	}

	virtual ~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		RELEASE_SOCKET(m_socket);		//释放socket资源，可能不必要
	}
	void Reset()
	{
		m_socket = INVALID_SOCKET;
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = m_uBufLength;
		ZeroMemory(m_szBuffer, MAX_BUF_LEN);
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
		m_uBufLength = MAX_RCV_BUF_LEN;
		m_szBuffer = new char[MAX_RCV_BUF_LEN];

		Reset();
	}
};

//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================

struct PER_SOCKET_CONTEXT
{
	SOCKET					m_socket;                   // 每一个客户端连接的Socket
	SOCKADDR_IN				m_clientAddr;               // 客户端的地址
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//接收上下文
	int						m_iDisconnectFlag;			//断开连接标识
	int						m_iSendPendingFlag;			//发送请求标识
	std::queue<PER_IO_CONTEXT*> m_queueIoContext;       //客户端发送数据请求的上下队列

	// 初始化
	PER_SOCKET_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
	}

	// 释放资源
	~PER_SOCKET_CONTEXT()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	void Reset()
	{
		//socket 由外部管理，此处不清理
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
		m_ReceiveContext.Reset();
		m_iDisconnectFlag = 0;
		m_iSendPendingFlag = 0;
		
		if(false == m_queueIoContext.empty())
		{
			MLOG("socket:%d上可能存在%d个IO资源泄露，请提前释放！", m_socket, m_queueIoContext.size());
		}
	}
};
