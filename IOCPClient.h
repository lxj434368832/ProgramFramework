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

class ConnectManage;
class ServerManage;
class ClientManage;

// 1、将所有类型的连接集中在一个地方。
// 2、负责初始化所有类型的连接。
// 3、统一创建iocp，统一创建iocp线程。

class IOCPClient
{
  public:
	  IOCPClient(ConnectManage *pCnntMng = NULL, 
		  ServerManage *pSrvMng = NULL, ClientManage *pClientMng = NULL);
	  virtual ~IOCPClient();

    bool InitIOCP(unsigned uThreadCount);

    //工作线程
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

	/*************************************************************************
	* function：开启一个连接需要考虑是否需要重连
	* param ip port: 需要连接的ip和端口号
	* param pIO:	 如果连接时要发送数据，需要传递pIO
	* param iRecnnt: 是否重连标识,小于0代表不需要重连
	* return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	*************************************************************************/
	unsigned StartConnect(std::string ip, u_short port, int iRecnnt = -1, const char* data = nullptr, int len = 0);

    //处理连接操作
    void HandConnectOperate(int iResult, PER_IO_CONTEXT* pIO);

    //处理客户端操作
    void HandClientOperate(int iResult, PER_IO_CONTEXT* pIO);

    //处理服务端操作
    void HandServerOperate(int iResult, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function：获取一个IOContext的结构
	* return:	IOContext的指针
	*************************************************************************/
    PER_IO_CONTEXT* GetIOContext();

	//回收利用IOContext
	void ReleaseIOContext(PER_IO_CONTEXT *pIO);

  private:
	  bool PostConnectEx(PER_IO_CONTEXT* pIO, SOCKADDR* serverAddr);

    //投递接受
	  bool PostAcceptEx(SOCKET listenSocket, EOperateType op);

    //
	void PostDisconnectEx(PER_IO_CONTEXT* pIO, EOperateType op);

    //
    void PostReceive(PER_IO_CONTEXT* pIO, EOperateType op);

    //发送
	void PostSend(PER_IO_CONTEXT* pIO, EOperateType op);

	//解包接收到的数据
	void UnpackReceivedData(PER_IO_CONTEXT* pIO, std::function<void(unsigned, const char*, unsigned)> HandData);

	void DoReceive(PER_IO_CONTEXT* pIO, int iResult, std::function<void(unsigned, const char*, unsigned)> HandData);

private:
	HANDLE 							m_hIOCP;		//完成端口
	unsigned						m_uThreadCount;	//线程个数
	HANDLE				 			*m_aThreadList;	//线程池列表
    mqw::ResourceManage<PER_IO_CONTEXT>	m_rscIO;		//IO资源管理

	ConnectManage					*m_pCnntMng;	//连接管理
	ServerManage					*m_pSrvMng;		//服务器管理
	ClientManage					*m_pClientMng;	//客户端管理
};

