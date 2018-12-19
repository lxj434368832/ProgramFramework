#include "../../IOCPCommunication/IOCPServer.h"
#include "../CommonDefine.h"
#include "CommunicationBusiness.h"
#include "../MainServer.h"
#include "../ManageBusiness/IManageBusiness.h"

CommunicationBusiness::CommunicationBusiness(IMainClient *main)
	:ICommunication(main)
{
	m_pIOCPServer = new IOCPServer((INetInterface*)main->GetManageBusiness()->GetUserInfoManage());
}

CommunicationBusiness::~CommunicationBusiness()
{
	RELEASE(m_pIOCPServer);
}

bool CommunicationBusiness::Start()
{
	ServerConfig &srvCfg = *m_srv->GetServerConfig();
	if (false == m_pIOCPServer->StartServer(srvCfg.usListenPort,
		srvCfg.uInitAcceptCount, srvCfg.uServerThreadCount))
		return false;

	return true;
}

void CommunicationBusiness::Stop()
{
	m_pIOCPServer->StopServer();
}
