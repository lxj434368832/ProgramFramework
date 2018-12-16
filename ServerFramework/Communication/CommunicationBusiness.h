#pragma once

#include "ICommunication.h"

class IOCPServer;

class CommunicationBusiness : public ICommunication
{
public:
	CommunicationBusiness(IMainServer *srv = nullptr);
	~CommunicationBusiness();


	bool Start() override;
	void Stop() override;

private:
	IOCPServer		*m_pIOCPServer;	//IOCP ·þÎñ¶Ë

};

