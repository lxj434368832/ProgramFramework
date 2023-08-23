#pragma once

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//������֪ͨ
	virtual void ConnectNotify(unsigned uKey, bool bSuccess = true) = 0;

	//�Ͽ�����֪ͨ
	virtual void DisConnectNotify(unsigned uKey) = 0;

	//�����������
	virtual void HandData(unsigned uKey, unsigned uMsgType, const char* data, unsigned length) = 0;
};
