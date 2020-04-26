#pragma once

#include "ICommunication.h"

class Communication : public ICommunication
{
public:
	Communication(IMainServer *pMain = nullptr);
	~Communication();
	PbMessageHandle* GetPbMessageHandle() override;
	TCPServer* GetTCPServer() override;

	bool Initialize() override;
	void Uninitialize() override;

	void RegisterMessageHandle(unsigned, std::function<void(const unsigned, SDataExchange*)>) override;

private:
	PbMessageHandle		*m_pPbMsgHandle;
	TCPServer			*m_pServer;
};
