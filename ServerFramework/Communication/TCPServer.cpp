#include "TCPServer.h"
#include "ICommunication.h"
#include "../CommonFile/CommonDefine.h"
#include "../MainServer/IMainServer.h"
#include "../Model/ModelManage/IModelManage.h"
#include "../Model/ModelManage/UserInfoManage.h"
#include "../Controller/ControllerManage/IControllerManage.h"
#include "../Component/MessageHandle/PbMessageHandle.h"
#include "../3rdParty/IOCPCommunication/include/IOCPServer.h"

TCPServer::TCPServer(IMainServer *pMain)
	:INetInterface()
{
	m_pMain = pMain;
	m_pIOCPServer = new IOCPServer();
	m_pPbMsgHandle = nullptr;
	m_pUserMng = nullptr;
}

TCPServer::~TCPServer()
{
	m_pMain = nullptr;
	RELEASE(m_pIOCPServer);
}

bool TCPServer::Initialize()
{
	m_pPbMsgHandle = m_pMain->GetCommunication()->GetPbMessageHandle();
	if (nullptr == m_pPbMsgHandle)
	{
		loge() << "获取消息处理失败！";
		return false;
	}

	m_pUserMng = m_pMain->GetModelManage()->GetUserInfoManage();
	if (nullptr == m_pUserMng)
	{
		loge() << "获取用户信息管理失败！";
		return false;
	}

	return true;
}

void TCPServer::Uninitialize()
{
	m_pPbMsgHandle = nullptr;
	m_pUserMng = nullptr;
}

bool TCPServer::StartServer()
{
	SServerConfig &srvCfg = *m_pMain->GetServerConfig();
	m_pIOCPServer->InitData(this);
	return m_pIOCPServer->StartServer(srvCfg.usListenPort,
		srvCfg.uInitAcceptCount, srvCfg.uServerThreadCount);
}

void TCPServer::StopServer()
{
	m_pIOCPServer->StopServer();
}

void TCPServer::SendData(UserKey uUserKey, unsigned uMsgType, const char * data, unsigned uLength)
{
	if (m_pIOCPServer)
	{
		m_pIOCPServer->SendData(uUserKey, uMsgType, data, uLength);
	}
}

void TCPServer::SendData(UserKey uUserKey, SPbMsg &msg)
{
	if (m_pIOCPServer)
	{
		m_pIOCPServer->SendData(uUserKey, msg.uMsgType, msg.strMsg.data(), msg.strMsg.length());
	}
}

void TCPServer::Disconnect(UserKey uUserKey)
{
	if (m_pIOCPServer)	m_pIOCPServer->Disconnect(uUserKey);
}

void TCPServer::ConnectNotify(UserKey uUserKey, bool bSuccess)
{
	if(m_pUserMng)
		m_pUserMng->AddUser(uUserKey);
}

void TCPServer::DisConnectNotify(UserKey uUserKey)
{
	if (m_pUserMng)
		m_pUserMng->DeleteUser(uUserKey);
}

void TCPServer::HandData(UserKey uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	if (m_pPbMsgHandle) 
		m_pPbMsgHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
