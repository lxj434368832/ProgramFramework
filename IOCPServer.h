#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"
#include "IOCPDef.h"
#include "ResourceManage.h"
#include "MLock.h"

class INetInterface;

// 1、将所有类型的连接集中在一个地方。
// 2、负责初始化所有类型的连接。
// 3、统一创建iocp，统一创建iocp线程。

class IOCPServier
{
  public:
	  IOCPServier(INetInterface *pSrvMng = nullptr);
	  virtual ~IOCPServier();

    bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function：  开启针对服务端的监听
	* param port: 本地监听的端口号
	* param iMaxServerCount:最大的连接个数
	* return:	  成功返回true,失败返回false.
	*************************************************************************/
    bool StartServerListen(u_short port, unsigned iMaxServerCount);

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(int key, std::string data);

protected:
	//处理服务端操作
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	void HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr);
    //投递接受
	  bool PostAcceptEx(SOCKET listenSocket);

    //
	void PostDisconnectEx(PER_IO_CONTEXT* pIO);

    //
    void PostReceive(PER_IO_CONTEXT* pIO);

    //发送
	void PostSend(PER_IO_CONTEXT* pIO, EOperateType op);

	//解包接收到的数据
	void UnpackReceivedData(PER_IO_CONTEXT* pIO, std::function<void(unsigned, const char*, unsigned)> HandData);

	void DoReceive(PER_IO_CONTEXT* pIO, int iResult, std::function<void(unsigned, const char*, unsigned)> HandData);

private:
	//工作线程
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

protected:
	enum 
	{
		SOCKET_CONTEXT_LOCK_COUNT = 100
	};
	const unsigned					m_uSocketContextLockCount = 100;
	HANDLE 							m_hIOCompletionPort;		//完成端口
	unsigned						m_uThreadCount;				//线程个数
	HANDLE				 			*m_aThreadList;				//线程池列表
	PER_SOCKET_CONTEXT				*m_pListenSocketContext;	//监听socket上下文
	std::map<SOCKET, PER_SOCKET_CONTEXT*>	m_mapConnectList;	//连接列表
	MLock	m_arraylckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];	//socket上下文锁
	MLock									m_lckConnect;		//连接列表锁
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	m_rscSocketContext;	//socket资源管理
    mqw::ResourceManage<PER_IO_CONTEXT>		m_rscIoContext;		//IO资源管理
	INetInterface *m_pNetInterface;								//服务管理器
};
