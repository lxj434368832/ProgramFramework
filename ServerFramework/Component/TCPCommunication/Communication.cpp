#include "Communication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainServer/MainServer.h"
#include "../MessageHandle/PbMessageHandle.h"
#include "TCPServer.h"

Communication::Communication(IMainServer *pMain)
	:ICommunication(pMain)
{
	m_pServer = new TCPServer(pMain);
}

Communication::~Communication()
{
	RELEASE(m_pServer);
}

TCPServer* Communication::GetTCPServer()
{
	return m_pServer;
}

bool Communication::Initialize()
{
	if (false == m_pServer->Initialize()) return false;
	return true;
}

void Communication::Uninitialize()
{
	m_pServer->Uninitialize();
}
