#include "../../../IOCPCommunication/IOCPServer.h"
#include "../../CommonFile/CommonDefine.h"
#include "CommunicationBusiness.h"
#include "../../MainServer/MainServer.h"
#include "../../Model/ModelManage/IModelManage.h"

CommunicationBusiness::CommunicationBusiness(IMainServer *srv)
	:ICommunication(srv)
{
	m_pIOCPServer = new IOCPServer((INetInterface*)srv->GetManageBusiness()->GetUserInfoManage());
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
