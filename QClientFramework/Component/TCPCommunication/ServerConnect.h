#pragma once
/*************************************************************************
* function：服务端连接管理
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "../../3rdParty/IOCPCommunication/include/INetInterface.h"

class MessageHandle;
class IOCPClient;
class ITCPCommunication;
class IMessageHandle;

//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ITCPCommunication *pCmmnt = nullptr);
	virtual ~ServerConnect();
	bool Start(unsigned uThreadCount);
	void Stop();

	/*************************************************************************
	* function：开启一个连接需要考虑是否需要重连
	* param ip port: 需要连接的ip和端口号
	* param pIO:	 如果连接时要发送数据，需要传递pIO
	* param iRecnnt: 是否重连标识,小于0代表不需要重连
	* return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	*************************************************************************/
	virtual bool AddConnect(unsigned uUserKey, std::string ip, ushort port, int iRecnnt = -1);

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(unsigned uUserKey, const char* data, unsigned uLength);

	/*************************************************************************
	* function： 断开连接，底层会回调DeleteUser函数
	* param key: 用户id
	* return:	 无
	*************************************************************************/
	void Disconnect(unsigned uUserKey);

private:
	//实现INetInterface接口
	//新服务用户成功连接通知
	void AddUser(unsigned uUserKey) override;

	//处理服务数据
	void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 删除用户
	void DeleteUser(unsigned uUserKey) override;

private:
	ITCPCommunication*	m_pCommunication;
	IMessageHandle*		m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP 客户端

};

