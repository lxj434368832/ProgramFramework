#include "../../3rdParty/IOCPCommunication/include/IOCPClient.h"
#include "ServerConnect.h"
#include "ITCPCommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageHandle\IMessageHandle.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "..\..\CommonFile\EnumDefine.h"

ServerConnect::ServerConnect(ITCPCommunication *pCmmnt)
	:INetInterface()
{
	m_pCommunication = pCmmnt;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pCommunication = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Start()
{
	m_pMsgModule = m_pCommunication->GetMainClient()->GetMessageHandle();
	if (nullptr == m_pMsgModule)
	{
		loge() << "获取消息处理模块失败！";
		return false;
	}

	ClientConfig &cfg = *m_pCommunication->GetMainClient()->GetClientConfig();
	if (false == m_pIOCPClient->StartClient(cfg.uIOCPThreadCount)) return false;

	for (int i = EST_CMD_SERVER; i < EST_SERVER_COUNT ; i++)
	{
		if (false == m_pIOCPClient->AddConnect(i, cfg.strServerIP, cfg.usServerPort, 1))
		{
			loge() << "添加对命令服务器的连接失败！";
			return false;
		}
	}
	return true;
}

void ServerConnect::Stop()
{
	m_pIOCPClient->StopClient();
	m_pMsgModule = nullptr;
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
	emit m_pCommunication->signalTcpConnectNotify(uUserKey);
}

void ServerConnect::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	uMsgType;

	if (m_pMsgModule)
	{
		m_pMsgModule->HandleProtobufMessage(uUserKey, data, length);
	}
}

void ServerConnect::DeleteUser(UserKey uUserKey)
{
	emit m_pCommunication->signalTcpDisconnectNotify(uUserKey);
}
