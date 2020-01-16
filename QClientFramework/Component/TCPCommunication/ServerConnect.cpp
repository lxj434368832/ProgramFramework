#include "ITCPCommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageHandle\IMessageHandle.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "ServerConnect.h"
#include "../../3rdParty/IOCPCommunication/include/IOCPClient.h"

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

bool ServerConnect::Start(unsigned uThreadCount)
{
	m_pMsgModule = m_pCommunication->GetMainClient()->GetMessageHandle();
	if (nullptr == m_pMsgModule)
	{
		loge() << "获取消息处理模块失败！";
		return false;
	}

	if (false == m_pIOCPClient->StartClient(uThreadCount)) return false;

	return true;
}

void ServerConnect::Stop()
{
	m_pIOCPClient->StopClient();
	m_pMsgModule = nullptr;
}

bool ServerConnect::AddConnect(unsigned uUserKey, std::string ip, ushort port, int iRecnnt)
{
	if (false == m_pIOCPClient->AddConnect(uUserKey, ip, port, iRecnnt))
	{
		loge() << "添加对命令服务器的连接失败！";
		return false;
	}
}

void ServerConnect::Send(unsigned uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, 0, data, uLength);
	}
}

void ServerConnect::Disconnect(unsigned uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void ServerConnect::AddUser(unsigned uUserKey)
{
	emit m_pCommunication->signalTcpConnectNotify(uUserKey);
}

void ServerConnect::HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	uMsgType;

	if (m_pMsgModule)
	{
		m_pMsgModule->HandleProtobufMessage(uUserKey, data, length);
	}
}

void ServerConnect::DeleteUser(unsigned uUserKey)
{
	emit m_pCommunication->signalTcpDisconnectNotify(uUserKey);
}
