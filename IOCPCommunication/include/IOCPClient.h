#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "IOCPBase.h"
#include "stdafx.h"

class IOCPClient :public IOCPBase
{
  public:
	  IOCPClient(INetInterface *pNet);
	  virtual ~IOCPClient();
	  bool StartClient(unsigned uThreadCount = 3);
	  void StopClient();

	  /*************************************************************************
	  * function：开启一个连接需要考虑是否需要重连
	  * param ip port: 需要连接的ip和端口号
	  * param pIO:	 如果连接时要发送数据，需要传递pIO
	  * param iRecnnt: 是否重连标识,小于0代表不需要重连
	  * return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	  *************************************************************************/
	  bool AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

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
};
