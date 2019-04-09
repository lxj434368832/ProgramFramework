#include "TCPCommunication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
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

ServerConnect * TCPCommunication::GetServerConnect()
{
	return m_pConnect;
}

bool TCPCommunication::Start()
{
	return m_pConnect->Start();
}

void TCPCommunication::Stop()
{
	m_pConnect->Stop();
}

void TCPCommunication::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	m_pConnect->Send(uUserKey, data, uLength);
}
