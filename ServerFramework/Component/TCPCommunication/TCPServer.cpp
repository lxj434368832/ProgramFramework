#include "TCPServer.h"
#include "ICommunication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainServer/IMainServer.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../MessageHandle/IMessageHandle.h"
#include "../../3rdParty/IOCPCommunication/include/IOCPServer.h"

TCPServer::TCPServer(IMainServer *pMain)
	:INetInterface()
{
	m_pMain = pMain;
	m_pIOCPServer = new IOCPServer(this);
	m_pMsgHandle = nullptr;
	m_pUserMng = nullptr;
}

TCPServer::~TCPServer()
{
	m_pMain = nullptr;
	RELEASE(m_pIOCPServer);
}

bool TCPServer::Initialize()
{
	m_pMsgHandle = m_pMain->GetMessageHandle();
	if (nullptr == m_pMsgHandle)
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
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调

	m_pMsgHandle = nullptr;
	m_pUserMng = nullptr;
}

bool TCPServer::StartServer()
{
	SServerConfig &srvCfg = *m_pMain->GetServerConfig();
	return m_pIOCPServer->StartServer(srvCfg.usListenPort,
		srvCfg.uInitAcceptCount, srvCfg.uServerThreadCount);
}

void TCPServer::StopServer()
{
	m_pIOCPServer->StopServer();
}

void TCPServer::SendData(UserKey uUserKey, unsigned uMsgType, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, uMsgType, data, uLength);
	}
}

void TCPServer::SendData(UserKey uUserKey, SPbMsg &msg)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, msg.uMsgType, msg.strMsg.data(), msg.strMsg.length());
	}
}

void TCPServer::Disconnect(UserKey uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void TCPServer::AddUser(UserKey uUserKey)
{
	if(m_pUserMng)
		m_pUserMng->AddUser(uUserKey);
}

void TCPServer::HandData(UserKey uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	if (m_pMsgHandle) 
		m_pMsgHandle->HandleProtobufMessage(uUserKey, uMsgType, data, length);
}

void TCPServer::DeleteUser(UserKey uUserKey)
{
	if (m_pUserMng)
		m_pUserMng->DeleteUser(uUserKey);
}
