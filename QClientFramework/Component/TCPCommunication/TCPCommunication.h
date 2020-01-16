#pragma once

#include "ITCPCommunication.h"
#include "../../CommonFile/TypeDefine.h"

class TCPCommunication : public ITCPCommunication
{
public:
    TCPCommunication(IMainClient *main = nullptr);
    ~TCPCommunication();

	bool Start() override;
	void Stop() override;

	bool ConnectServer() override;
	//此处userKey其实是服务器类型
	void SendData(UserKey uUserKey, const char* data, unsigned uLength) override;

private:
	ServerConnect*		m_pConnect;

};

