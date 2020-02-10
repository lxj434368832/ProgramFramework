#pragma once

#include "../../3rdParty/IOCPCommunication/include/INetInterface.h"

class IMainServer;
class IOCPServer;
class UserInfoManage;
class IMessageHandle;
struct SPbMsg;

class TCPServer : public INetInterface
{
public:
	TCPServer(IMainServer*);
	~TCPServer();

	bool Initialize();
	void Uninitialize();

	bool StartServer();
	void StopServer();

	/*************************************************************************
	* function： 发送数据
	* param uMsgType: 消息类型
	* param key:	  用户id
	* param data:	 需要发送的数据
	*************************************************************************/
	void SendData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function： 发送数据
	* param uMsgType: 消息类型
	* param msg:	 需要发送的数据
	*************************************************************************/
	void SendData(UserKey uUserKey, SPbMsg &msg);

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
