#include "Communication.h"
#include "../CommonFile/CommonDefine.h"
#include "../MainServer/MainServer.h"
#include "../Component/MessageHandle/PbMessageHandle.h"
#include "TCPServer.h"

Communication::Communication(IMainServer *pMain)
	:ICommunication(pMain)
{
	m_pPbMsgHandle = new PbMessageHandle(pMain);
	m_pServer = new TCPServer(pMain);
}

Communication::~Communication()
{
	RELEASE(m_pServer);
	RELEASE(m_pPbMsgHandle);
}

PbMessageHandle* Communication::GetPbMessageHandle()
{
	return m_pPbMsgHandle;
}

TCPServer* Communication::GetTCPServer()
{
	return m_pServer;
}

bool Communication::Initialize()
{
	if (false == m_pPbMsgHandle->Initialize(m_pMain->GetServerConfig()->uMessageThreadCount)) return false;
	if (false == m_pServer->Initialize()) return false;
	return true;
}

void Communication::Uninitialize()
{
	m_pServer->Uninitialize();
	m_pPbMsgHandle->Uninitialize();
}

void Communication::RegisterMessageHandle(unsigned msgType, std::function<void(const unsigned, SDataExchange*)> fun)
{
	m_pPbMsgHandle->RegisterMessageFunction(msgType, fun);
}
