#pragma once
/*************************************************************************
* function：iocp公共定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"
#include <afxtempl.h>

#define MAX_BUF_LEN 512 //512B
#define RECEIVE_BUF_COUNT 10


// 在完成端口上投递的I/O操作的类型
enum EOperateType
{
	EOP_UNKNOWN,
	EOP_CNNT_CONNECT,
	EOP_CNNT_DISCONNECT,
	EOP_CNNT_RECEIVE,
	EOP_CNNT_SEND,
	EOP_CNNT_OPERATE,
	EOP_CLIENT_ACCEPT,
	EOP_CLIENT_DISCONNECT,
	EOP_CLIENT_RECEIVE,
	EOP_CLIENT_SEND,
	EOP_CLIENT_OPRATE,
	EOP_SRV_ACCEPT,
	EOP_SRV_DISCONNECT,
	EOP_SRV_RECEIVE,
	EOP_SRV_SEND,
	EOP_SRV_OPERATE
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

	~PER_IO_CONTEXT()
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

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET					m_Socket;                   // 每一个客户端连接的Socket
	SOCKADDR_IN				m_ClientAddr;               // 客户端的地址
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//接收上下文

	CArray<PER_IO_CONTEXT*> m_arrayIoContext;             // 客户端网络操作的上下文数据，
														   // 也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的

	// 初始化
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
	}

	// 释放资源
	~_PER_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		// 释放掉所有的IO上下文数据
		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			delete m_arrayIoContext.GetAt(i);
		}
		m_arrayIoContext.RemoveAll();
	}

	// 获取一个新的IoContext
	PER_IO_CONTEXT* GetNewIoContext()
	{
		PER_IO_CONTEXT* p = new PER_IO_CONTEXT;

		m_arrayIoContext.Add(p);

		return p;
	}

	// 从数组中移除一个指定的IoContext
	void RemoveContext(PER_IO_CONTEXT* pContext)
	{
		ASSERT(pContext != NULL);

		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			if (pContext == m_arrayIoContext.GetAt(i))
			{
				delete pContext;
				pContext = NULL;
				m_arrayIoContext.RemoveAt(i);
				break;
			}
		}
	}

}PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;
