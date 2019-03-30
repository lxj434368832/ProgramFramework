#pragma once

#include "ICommunication.h"

class MainCommunication : public ICommunication
{
public:
	MainCommunication(IMainClient *main = nullptr);
	~MainCommunication();
	ServerConnect* GetServerConnect() override;

	bool Start() override;
	void Stop() override;
	void SendData(UserKey uUserKey, const char* data, unsigned uLength) override;

private:
	ServerConnect*		m_pConnect;

};

