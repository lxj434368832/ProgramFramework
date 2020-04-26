#pragma once
/*************************************************************************
* function：iocp公共定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/

#include <WinSock2.h>
#include <MSWSock.h>
#include <queue>
#include <atomic>

#define MAX_BUF_LEN 512			//发送缓冲区大小512B
#define MAX_RCV_BUF_LEN 5120	//接收缓冲区大小5K
#define CHECK_VALUE 0xfafbfcfd	//网络包检测值

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
	unsigned		uCheck = CHECK_VALUE;	//主要用于错包检测
	unsigned		uMsgType;
	unsigned long	ulBodyLength;
};
#pragma pack(pop)

//单IO数据结构体定义(用于每一个重叠操作的参数)
struct PER_IO_CONTEXT
{
	OVERLAPPED	m_overlapped;
	unsigned	m_uUserKey;
	SOCKET		m_socket;
	EOperateType m_oprateType;
	WSABUF		m_wsaBuf;
	char		*m_szBuffer;
	unsigned	m_uBufLength;
	unsigned	m_uDataLength;

	PER_IO_CONTEXT(unsigned BufLen = MAX_BUF_LEN)
	{
		ZeroMemory(&m_overlapped, sizeof(m_overlapped));
		m_uUserKey = 0;
		m_socket = INVALID_SOCKET;
		m_uBufLength = BufLen;
		m_szBuffer = new char[BufLen];

		Reset();
	}

	virtual ~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		m_szBuffer = nullptr;
	}

	void Reset()
	{
		m_uUserKey = 0;
		m_socket = INVALID_SOCKET;
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = 0;
		ZeroMemory(m_szBuffer, m_uBufLength);
		m_uDataLength = 0;
	}
};

struct PER_RECEIVE_IO_CONTEXT :public PER_IO_CONTEXT
{
	PER_RECEIVE_IO_CONTEXT()
		:PER_IO_CONTEXT(MAX_RCV_BUF_LEN)
	{
	}
};

//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================

struct PER_SOCKET_CONTEXT
{
	unsigned				m_uUserKey;					//每个socket对应的UserKey
	SOCKET					m_socket;                   // 每一个客户端连接的Socket
	SOCKADDR_IN				m_clientAddr;               // 客户端的地址
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//接收上下文
	std::atomic_int			m_iDisconnectFlag;			//断开连接标识，1表示断开，准备重用，2表示断开，不重用
	std::queue<PER_IO_CONTEXT*> m_queueIoContext;       //客户端发送数据请求的上下队列

	// 初始化
	PER_SOCKET_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
	}

	// 释放资源
	~PER_SOCKET_CONTEXT()
	{
		if (INVALID_SOCKET != m_socket)	//释放socket资源
		{
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	void Reset()
	{
		//socket 由外部管理，此处不清理
		m_uUserKey = 0;
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
		m_ReceiveContext.Reset();
		m_iDisconnectFlag = 0;
		
		if(false == m_queueIoContext.empty())
		{
			printf("socket:%d上可能存在%d个IO资源泄露，请提前释放！", m_socket, m_queueIoContext.size());
		}
	}
};



#include "..\Framework\include\ResourceManage.h"
#include "..\Framework\include\MLock.h"
#include "INetInterface.h"
#include <thread>

struct IOCPBaseData
{
	enum
	{
		SOCKET_RESOURCE_COUNT = 100,
		IO_RESOURCE_COUNT = 40,
		SOCKET_CONTEXT_LOCK_COUNT = 10
	};

	HANDLE 							hIOCompletionPort;		//完成端口
	unsigned						uThreadCount;			//事务线程个数
	std::thread				 		*aThreadList;			//事务线程池列表

	std::atomic_uint						uUserNum;		//用户编号
	std::map<unsigned, PER_SOCKET_CONTEXT*>	mapConnectList;	//连接列表
	MLock									lckConnectList;	//连接列表锁

	MLock	aLckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];		//socket上下文锁
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	rscSocketContext;	//socket资源管理
	mqw::ResourceManage<PER_IO_CONTEXT>		rscIoContext;		//IO资源管理
	INetInterface							*pNetInterface;		//网络接口
	std::atomic_int							iExitUserCount;		//退出用户计数

	IOCPBaseData(INetInterface *pNet) :
		pNetInterface(pNet),
		hIOCompletionPort(INVALID_HANDLE_VALUE),
		rscSocketContext(SOCKET_RESOURCE_COUNT),
		rscIoContext(IO_RESOURCE_COUNT)
	{
		uUserNum = 100;
		uThreadCount = 0;
		aThreadList = NULL;
		iExitUserCount = 0;
	}

	~IOCPBaseData()
	{
		pNetInterface = nullptr;
	}
};

struct IOCPServerData : public IOCPBaseData
{
	bool						bStart;
	PER_SOCKET_CONTEXT			*pListenSocketContext;	//监听socket上下文
	
	IOCPServerData(INetInterface *pNet):
		IOCPBaseData(pNet)
	{
		bStart = false;
		pListenSocketContext = nullptr;
	}
};