#pragma once

#include "../3rdParty/IOCPCommunication/include/INetInterface.h"

class IMainServer;
class IOCPServer;
class UserInfoManage;
class PbMessageHandle;
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
	// 新用户连接通知
	void ConnectNotify(UserKey uUserKey, bool bSuccess) override;

	// 用户断开连接通知
	void DisConnectNotify(UserKey uUserKey) override;

	// 处理服务数据
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	IMainServer			*m_pMain;
	IOCPServer			*m_pIOCPServer;	//IOCP 服务端
	PbMessageHandle		*m_pPbMsgHandle;
	UserInfoManage		*m_pUserMng;
};
