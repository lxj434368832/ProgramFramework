#include "TCPCommunication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "../../CommonFile/TypeDefine.h"
#include "ServerConnect.h"

TCPCommunication::TCPCommunication(IMainClient *main)
	:ITCPCommunication(main)
{
	m_pConnect = new ServerConnect(this);
}

TCPCommunication::~TCPCommunication()
{
	RELEASE(m_pConnect);
}

bool TCPCommunication::Start()
{
	ClientConfig *cfg = m_pMain->GetClientConfig();
	return m_pConnect->Start(cfg->uIOCPThreadCount);
}

void TCPCommunication::Stop()
{
	m_pConnect->Stop();
}

bool TCPCommunication::ConnectServer()
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

void TCPCommunication::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	m_pConnect->Send(uUserKey, data, uLength);
}
