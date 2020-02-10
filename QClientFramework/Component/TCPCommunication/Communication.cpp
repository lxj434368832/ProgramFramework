#include "Communication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "../../CommonFile/TypeDefine.h"
#include "ServerConnect.h"

Communication::Communication(IMainClient *main)
	:ICommunication(main)
{
	m_pConnect = new ServerConnect(this);
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
	return m_pConnect->Initialize(cfg->uIOCPThreadCount);
}

void Communication::Uninitialize()
{
	m_pConnect->Uninitialize();
}

bool Communication::ConnectServer()
{
	ClientConfig *cfg = m_pMain->GetClientConfig();
	
	for (int i = EST_CMD_SERVER; i < EST_SERVER_COUNT; i++)
	{
		if (false == m_pConnect->AddConnect(i, cfg->strServerIP, cfg->usServerPort, 1))
		{
			loge() << "添加对命令服务器的连接失败！";
			return false;
		}
	}
}
