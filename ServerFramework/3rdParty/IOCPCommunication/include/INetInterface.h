#pragma once

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//���û�����֪ͨ
	virtual void ConnectNotify(UserKey uUserKey, bool bSuccess = true) = 0;

	//�û��Ͽ�����֪ͨ
	virtual void DisConnectNotify(UserKey uUserKey) = 0;

	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;
};
