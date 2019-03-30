#include "MainCommunication.h"
#include "../../include/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "ServerConnect.h"

MainCommunication::MainCommunication(IMainClient *main)
	:ICommunication(main)
{
	m_pConnect = new ServerConnect(this);
}

MainCommunication::~MainCommunication()
{
	RELEASE(m_pConnect);
}

ServerConnect * MainCommunication::GetServerConnect()
{
	return m_pConnect;
}

bool MainCommunication::Start()
{
	return m_pConnect->Start();
}

void MainCommunication::Stop()
{
	m_pConnect->Stop();
}

void MainCommunication::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	m_pConnect->Send(uUserKey, data, uLength);
}
