#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company: 
*************************************************************************/

#include "IOCPBase.h"

struct IOCPServerData;

class IOCPServer : public IOCPBase
{
public:
	IOCPServer(INetInterface *pNet = nullptr);
	virtual ~IOCPServer();

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	void StopServer();

	//心跳检测函数，由主线程定时调用。
	void StartHeartbeatCheck();

	/*************************************************************************
	* function： 发送数据，外部回调
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void SendData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function： 断开连接，外部回调
	* param key: 用户id
	*************************************************************************/
	void Disconnect(unsigned uUserKey) override;

private:
	/*************************************************************************
	* function：  开启针对服务端的监听
	* param port: 本地监听的端口号
	* param iMaxServerCount:最大的连接个数
	* return:	  成功返回true,失败返回false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount) override;

private:
	IOCPServerData *d;
};
