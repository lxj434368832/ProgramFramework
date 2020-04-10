#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/


struct PER_SOCKET_CONTEXT;
struct PER_IO_CONTEXT;
struct IOCPBaseData;
class INetInterface;

class IOCPBase
{
public:
	IOCPBase(INetInterface *pNet);
	IOCPBase(IOCPBaseData *d);
	virtual ~IOCPBase();

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
	virtual bool AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

protected:
	bool InitIOCP(unsigned uThreadCount);

	void UninitIOCP();

	//处理服务端操作
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);
	//投递连接
	bool PostConnectEx(PER_SOCKET_CONTEXT *pSkContext);
	//处理连接事务
	void HandConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//投递接受
	bool PostAcceptEx(SOCKET listenSocket);
	void HandAccept(int iResult, PER_SOCKET_CONTEXT *pListenSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void HandReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//解包接收到的数据
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function： 发送数据，外部回调
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	//打包数据
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//发送
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext);

	void HandSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	/*************************************************************************
	* function： 断开连接，外部回调
	* param key: 用户id
	*************************************************************************/
	void Disconnect(unsigned uUserKey);
	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void ReCycleSocketRsc(PER_SOCKET_CONTEXT * pSkContext, PER_IO_CONTEXT* pIO);

	void HandDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

private:
	void WorkThread();

protected:
	IOCPBaseData *d;
};
