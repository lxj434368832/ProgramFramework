#include "Communication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "ServerConnect.h"

Communication::Communication(IMainClient *main)
	:ICommunication(main)
{
	m_pConnect = new ServerConnect(main);
}

Communication::~Communication()
{
	RELEASE(m_pConnect);
}

ServerConnect* Communication::GetServerConnect()
{
	return m_pConnect;
}

bool Communication::Initialize()
{
	ClientConfig *cfg = m_pMain->GetClientConfig();
	return m_pConnect->Initialize(nullptr, cfg->uServerThreadCount);
}

void Communication::Uninitialize()
{
	m_pConnect->Uninitialize();
}
