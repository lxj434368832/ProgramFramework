#pragma once

#include <map>
#include <functional>

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

	//注册回调函数
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//通知服务端新用户连接
	virtual void AddUser(UserKey uUserKey);
	//连接结果通知
	virtual void ConnectResult(UserKey uUserKey, bool bSuccess);
	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length);

	//通讯层回调删除用户
	virtual void DeleteUser(UserKey uUserKey);

protected:

	SendDataCallback				m_funSendData;		//发送数据回调
	DisconnectCallback				m_fuDisconnect;		//主动断开连接回调
};

