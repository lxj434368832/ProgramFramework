#include "ICommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageHandle\IMessageHandle.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "ServerConnect.h"
#include <windows.h>
#include "../../3rdParty/IOCPCommunication/include/IOCPClient.h"

ServerConnect::ServerConnect(ICommunication *pCmmnt)
	:INetInterface()
{
	m_pCmmnt = pCmmnt;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pCmmnt = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Initialize(unsigned uThreadCount)
{
	m_pMsgModule = m_pCmmnt->GetMainClient()->GetMessageHandle();
	if (nullptr == m_pMsgModule)
	{
		loge() << "获取消息处理模块失败！";
		return false;
	}

	if (false == m_pIOCPClient->StartClient(uThreadCount)) return false;

	return true;
}

void ServerConnect::Uninitialize()
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

void ServerConnect::SendData(UserKey uUserKey, SPbMsg &msg)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, msg.uMsgType, msg.strMsg.data(), msg.strMsg.length());
	}
}

void ServerConnect::Send(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, uMsgType, data, uLength);
	}
}

void ServerConnect::Disconnect(unsigned uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void ServerConnect::ConnectNotify(UserKey uUserKey, bool bSuccess)
{
	emit m_pCmmnt->signalTcpConnectNotify(uUserKey, bSuccess);
}

void ServerConnect::HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	if (m_pMsgModule)
	{
		m_pMsgModule->HandleProtobufMessage(uUserKey, uMsgType, data, length);
	}
}

void ServerConnect::DeleteUser(unsigned uUserKey)
{
	emit m_pCmmnt->signalTcpDisconnectNotify(uUserKey);
}
