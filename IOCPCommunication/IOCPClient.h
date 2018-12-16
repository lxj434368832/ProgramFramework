#pragma once
/*************************************************************************
* function：iocp通讯定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "IOCPBase.h"

class IOCPClient :public IOCPBase
{
  public:
	  IOCPClient(INetInterface *pNet);
	  virtual ~IOCPClient();

	  /*************************************************************************
	  * function：开启一个连接需要考虑是否需要重连
	  * param ip port: 需要连接的ip和端口号
	  * param pIO:	 如果连接时要发送数据，需要传递pIO
	  * param iRecnnt: 是否重连标识,小于0代表不需要重连
	  * return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	  *************************************************************************/
	  bool StartConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

	  void StopClient();
	  //开启心跳发送线程
	  void StartHeartbeatSend();
};

