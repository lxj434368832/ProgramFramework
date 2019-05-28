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
	bool Start();
	void Stop();

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(UserKey uUserKey, const char* data, unsigned uLength);

	/*************************************************************************
	* function： 断开连接，底层会回调DeleteUser函数
	* param key: 用户id
	* return:	 无
	*************************************************************************/
	void Disconnect(UserKey uUserKey);

private:
	//实现INetInterface接口
	//新服务用户成功连接通知
	void AddUser(UserKey uUserKey) override;

	//处理服务数据
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 删除用户
	void DeleteUser(UserKey uUserKey) override;

private:
	ITCPCommunication*	m_pCommunication;
	IMessageHandle*		m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP 客户端

};

