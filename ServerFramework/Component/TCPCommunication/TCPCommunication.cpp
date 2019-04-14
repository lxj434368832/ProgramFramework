#include "TCPCommunication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainServer/MainServer.h"
#include "TCPServer.h"

TCPCommunication::TCPCommunication(IMainServer *pMain)
	:ITCPCommunication(pMain)
{
	m_pServer = new TCPServer(pMain);
}

TCPCommunication::~TCPCommunication()
{
	RELEASE(m_pServer);
}

bool TCPCommunication::Start()
{
	return m_pServer->Start();
}

void TCPCommunication::Stop()
{
	m_pServer->Stop();
}

void TCPCommunication::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	m_pServer->SendData(uUserKey, data, uLength);
}
