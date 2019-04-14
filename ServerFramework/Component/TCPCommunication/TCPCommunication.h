#pragma once

#include "ITCPCommunication.h"

class TCPServer;

class TCPCommunication : public ITCPCommunication
{
public:
	TCPCommunication(IMainServer *pMain = nullptr);
	~TCPCommunication();

	bool Start() override;
	void Stop() override;
	virtual void SendData(UserKey uUserKey, const char* data, unsigned uLength) override;

private:
	TCPServer	*m_pServer;
};

