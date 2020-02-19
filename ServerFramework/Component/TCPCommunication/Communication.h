#pragma once

#include "ICommunication.h"

class Communication : public ICommunication
{
public:
	Communication(IMainServer *pMain = nullptr);
	~Communication();

	TCPServer* GetTCPServer() override;

	bool Initialize() override;
	void Uninitialize() override;

private:
	TCPServer		*m_pServer;
};