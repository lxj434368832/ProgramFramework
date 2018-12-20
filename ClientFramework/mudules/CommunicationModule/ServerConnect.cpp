#include "ServerConnect.h"
#include "..\..\..\IOCPCommunication\IOCPClient.h"
#include "MainCommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageModule\IMessage.h"
#include "..\MessageModule\ProtobufMsgFactory.h"
#include "..\..\include\TypeDefine.h"


ServerConnect::ServerConnect(ICommunication *pCmmnt)
	:INetInterface(),
	m_rscUser(SERVER_COUNT)
{
	m_pCommunication = pCmmnt;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pCommunication = nullptr;
	m_pMsgFctry = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Start()
{
	m_pMsgFctry = m_pCommunication->GetMainClient()->GetMessageModule()->GetProtobufMsgFactory();
	if (nullptr == m_pMsgFctry)
	{
		loge() << "获取消息工厂函数失败！";
		return false;
	}

	ClientConfig &cfg = *m_pCommunication->GetMainClient()->GetClientConfig();
	if (false == m_pIOCPClient->StartClient(cfg.uIOCPThreadCount)) return;

	if (false == m_pIOCPClient->AddConnect(EST_CMD_SERVER, cfg.strServerIP, cfg.usServerPort, 1))
	{
		loge() << "添加对命令服务器的连接失败！";
		return false;
	}
	return true;

}

void ServerConnect::Stop()
{
	m_pMsgFctry = nullptr;
}

void ServerConnect::Send(UserKey uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, 0, data, uLength);
	}
}

void ServerConnect::Disconnect(UserKey uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void ServerConnect::AddUser(UserKey uUserKey)
{

}

void ServerConnect::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void ServerConnect::DeleteUser(UserKey uUserKey)
{

}
