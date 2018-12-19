#include "MainCommunication.h"
#include "../../include/CommonDefine.h"
#include "../../MainClient/IMainClient.h"

MainCommunication::MainCommunication(IMainClient *main)
	:ICommunication(main)
{
	//m_pIOCPServer = new IOCPServer((INetInterface*)main->GetManageBusiness()->GetUserInfoManage());
}

MainCommunication::~MainCommunication()
{
	RELEASE(m_pIOCPClient);
}

bool MainCommunication::Start()
{
	ClientConfig &srvCfg = *m_main->GetClientConfig();
	//m_pIOCPClient->initIOCP();
	//if (false == m_pIOCPClient->StartClient(srvCfg.usListenPort,
	//	srvCfg.uInitAcceptCount, srvCfg.uServerThreadCount))
	//	return false;

	return true;
}

void MainCommunication::Stop()
{
	//m_pIOCPServer->StopServer();
}
