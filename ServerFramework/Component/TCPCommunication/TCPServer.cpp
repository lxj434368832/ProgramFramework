#include "TCPServer.h"
#include "ITCPCommunication.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainServer/IMainServer.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../MessageHandle/IMessageHandle.h"
#include "../../../IOCPCommunication/IOCPServer.h"

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

bool TCPServer::Start()
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

	ServerConfig &srvCfg = *m_pMain->GetServerConfig();
	if (false == m_pIOCPServer->StartServer(srvCfg.usListenPort,
		srvCfg.uInitAcceptCount, srvCfg.uServerThreadCount))
		return false;

	return true;
}

void TCPServer::Stop()
{
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调
	m_pIOCPServer->StopServer();
	m_pMsgHandle = nullptr;
	m_pUserMng = nullptr;
}

void TCPServer::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, 0, data, uLength);
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
	uMsgType;
	if (m_pMsgHandle) m_pMsgHandle->HandleProtobufMessage(uUserKey, data, length);
}

void TCPServer::DeleteUser(UserKey uUserKey)
{
	if (m_pUserMng)
		m_pUserMng->DeleteUser(uUserKey);
}
