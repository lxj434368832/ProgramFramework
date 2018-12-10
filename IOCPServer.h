#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company: 
*************************************************************************/

#include "stdafx.h"
#include "IOCPDef.h"
#include "ResourceManage.h"
#include "MLock.h"

class INetInterface;

class IOCPServier
{
public:
	IOCPServier(INetInterface *pSrvMng = nullptr);
	virtual ~IOCPServier();

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	bool StopServer();

	//心跳检测函数，由主线程定时调用。
	void HeartbeatCheck();

	bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function：  开启针对服务端的监听
	* param port: 本地监听的端口号
	* param iMaxServerCount:最大的连接个数
	* return:	  成功返回true,失败返回false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount);

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);
	
	void Disconnect(unsigned uUserKey);

protected:
	//处理服务端操作
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//投递接受
	bool PostAcceptEx(SOCKET listenSocket);

	void DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//解包接收到的数据
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);
	//打包数据
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//发送
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);


private:
	//工作线程
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

protected:
	enum 
	{
		SOCKET_CONTEXT_LOCK_COUNT = 100
	};

	HANDLE 							m_hIOCompletionPort;		//完成端口
	unsigned						m_uThreadCount;				//线程个数
	HANDLE				 			*m_aThreadList;				//线程池列表
	PER_SOCKET_CONTEXT				*m_pListenSocketContext;	//监听socket上下文
	std::map<SOCKET, PER_SOCKET_CONTEXT*>	m_mapConnectList;	//连接列表
	MLock									m_lckConnectList;	//连接列表锁

	MLock	m_aLckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];		//socket上下文锁
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	m_rscSocketContext;	//socket资源管理
    mqw::ResourceManage<PER_IO_CONTEXT>		m_rscIoContext;		//IO资源管理
	INetInterface *m_pNetInterface;								//服务管理器
};
