#pragma once

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//新用户连接通知
	virtual void ConnectNotify(UserKey uUserKey, bool bSuccess = true) = 0;

	//用户断开连接通知
	virtual void DisConnectNotify(UserKey uUserKey) = 0;

	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;
};
