#pragma once

#include "../../../IOCPCommunication/INetInterface.h"

class IMainServer;
class IOCPServer;
class UserInfoManage;
class IMessageHandle;

class TCPServer : public INetInterface
{
public:
	TCPServer(IMainServer*);
	~TCPServer();

	bool Start();
	void Stop();

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void SendData(UserKey uUserKey, const char* data, unsigned uLength);

	void Disconnect(UserKey uUserKey);

private:
	//实现INetInterface接口
	//新服务用户成功连接通知
	virtual void AddUser(UserKey uUserKey) override;

	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 删除用户
	virtual void DeleteUser(UserKey uUserKey) override;

private:
	IMainServer			*m_pMain;
	IOCPServer			*m_pIOCPServer;	//IOCP 服务端
	IMessageHandle		*m_pMsgHandle;
	UserInfoManage		*m_pUserMng;
};

