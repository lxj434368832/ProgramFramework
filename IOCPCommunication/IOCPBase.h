#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/

#include "IOCPDef.h"
#include "../Framework/ResourceManage.h"
#include "../Framework/MLock.h"

class INetInterface;

class IOCPBase
{
public:
	IOCPBase(INetInterface *pNet);
	virtual ~IOCPBase();

	bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function：  开启针对服务端的监听
	* param port: 本地监听的端口号
	* param iMaxServerCount:最大的连接个数
	* return:	  成功返回true,失败返回false.
	*************************************************************************/
	virtual bool StartServerListen(u_short port, unsigned iMaxConnectCount);

	/*************************************************************************
	* function：开启一个连接需要考虑是否需要重连
	* param ip port: 需要连接的ip和端口号
	* param pIO:	 如果连接时要发送数据，需要传递pIO
	* param iRecnnt: 是否重连标识,小于0代表不需要重连
	* return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	*************************************************************************/
	virtual bool StartConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

protected:
	void UninitIOCP();

	//处理服务端操作
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	bool PostConnectEx(PER_SOCKET_CONTEXT *pSkContext);

	void DoConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//投递接受
	bool PostAcceptEx(SOCKET listenSocket);

	void DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//解包接收到的数据
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	//打包数据
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//发送
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	void Disconnect(unsigned uUserKey);
	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

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
	INetInterface *m_pNetInterface;								//网络接口
};

