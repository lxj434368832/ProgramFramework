#pragma once

#include <map>
#include <functional>
#include "UserInfo.h"

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();
	//注册回调函数
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//新服务用户成功连接通知
	virtual void AddUser(unsigned uUserKey);

	//处理服务数据
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(unsigned uUserKey);

protected:
	SendDataCallback	m_funSendData;		//发送数据回调
	DisconnectCallback	m_fuDisconnect;		//主动断开连接回调
	std::map<unsigned, UserInfo> m_mapUserList;

};

