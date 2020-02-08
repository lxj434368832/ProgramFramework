#include "MainServer.h"
#include "../3rdParty/Framework/include/ConfigManage.h"
#include "../3rdParty/Framework/include/Utils.h"
#include "../CommonFile//CommonDefine.h"
#include "../Model/ModelManage/ModelManage.h"
#include "../Controller/ControllerManage/ControllerManage.h"
#include "../Component/MessageHandle/MessageHandle.h"
#include "../Component/TCPCommunication/Communication.h"

MainServer::MainServer()
{
	m_pModel = new ModelManage(this);
	m_pMessage = new MessageHandle(this);
	m_pCommunication = new Communication(this);
	m_pController = new ControllerManage(this);
}

MainServer::~MainServer()
{
	RELEASE(m_pCommunication);
	RELEASE(m_pMessage);
	RELEASE(m_pController);
	RELEASE(m_pModel);
}

IModelManage* MainServer::GetModelManage()
{
	return m_pModel;
}

IControllerManage* MainServer::GetControllerManage()
{
	return m_pController;
}

IMessageHandle* MainServer::GetMessageHandle()
{
	return m_pMessage;
}

ICommunication* MainServer::GetCommunication()
{
	return m_pCommunication;
}

SServerConfig* MainServer::GetServerConfig()
{
	return &m_srvConfig;
}

bool MainServer::StartServer()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pModel->Initialize()) return false;
	if (false == m_pMessage->Initialize()) return false;
	if (false == m_pCommunication->Initialize()) return false;
	if (false == m_pController->Initialize()) return false;
	if (false == m_pCommunication->StartServer()) return false;
	return true;
}

void MainServer::StopServer()
{
	m_pCommunication->StopServer();
	m_pController->Uninitialize();
	m_pCommunication->Uninitialize();
	m_pMessage->Uninitialize();
	m_pModel->Uninitialize();
}

bool MainServer::ReadConfigFile()
{
	std::string strConfigPath = Utils::get_module_path(nullptr, "\\config\\ServerConfig.ini");
	ConfigManage cfgMng(strConfigPath);
	m_srvConfig.usListenPort	= cfgMng.GetValueInt("Server", "ListenPort", 0);
	m_srvConfig.uServerThreadCount = cfgMng.GetValueInt("Server", "ServerThreadCount", 0);
	m_srvConfig.uMessageThreadCount = cfgMng.GetValueInt("Server", "MessageThreadCount", 0);
	m_srvConfig.uInitAcceptCount = cfgMng.GetValueInt("Server", "InitAcceptCount", 0);
	m_srvConfig.uHeartbeatTime = cfgMng.GetValueInt("Server", "HeartbeatTime", 0);

	if (false == m_srvConfig.CheckValid())
	{
		loge() << "读取配置文件发生错误，请检查配置文件！";
		return false;
	}
	return true;
}
