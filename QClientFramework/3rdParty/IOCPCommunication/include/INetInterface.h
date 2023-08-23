#pragma once

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//新连接通知
	virtual void ConnectNotify(unsigned uKey, bool bSuccess = true) = 0;

	//断开连接通知
	virtual void DisConnectNotify(unsigned uKey) = 0;

	//处理服务数据
	virtual void HandData(unsigned uKey, unsigned uMsgType, const char* data, unsigned length) = 0;
};
