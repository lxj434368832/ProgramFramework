#pragma once

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//通知服务端新用户连接
	virtual void AddUser(UserKey uUserKey){}

	//连接结果通知
	virtual void ConnectNotify(UserKey uUserKey, bool bSuccess){}

	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;

	//通讯层回调删除用户
	virtual void DeleteUser(UserKey uUserKey) = 0;
};


