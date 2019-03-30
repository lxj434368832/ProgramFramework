#pragma once

#include "ITCPCommunication.h"
#include "../../CommonFile/TypeDefine.h"

class TCPCommunication : public ITCPCommunication
{
public:
    TCPCommunication(IMainClient *main = nullptr);
    ~TCPCommunication();
	ServerConnect* GetServerConnect() override;

	bool Start() override;
	void Stop() override;
    void SendData(UserKey uUserKey, const char* data, unsigned uLength);

private:
	ServerConnect*		m_pConnect;

};

