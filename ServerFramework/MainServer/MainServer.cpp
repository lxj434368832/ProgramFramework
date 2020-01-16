#include "MainServer.h"
#include "../3rdParty/Framework/include/ConfigManage.h"
#include "../3rdParty/Framework/include/Utils.h"
#include "../CommonFile//CommonDefine.h"
#include "../Model/ModelManage/ModelManage.h"
#include "../Controller/ControllerManage/ControllerManage.h"
#include "../Component/MessageHandle/MessageHandle.h"
#include "../Component/TCPCommunication/TCPCommunication.h"

MainServer::MainServer()
{
	m_pModel = new ModelManage(this);
	m_pMessage = new MessageHandle(this);
	m_pCommunication = new TCPCommunication(this);
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

ITCPCommunication* MainServer::GetCommunication()
{
	return m_pCommunication;
}

ServerConfig* MainServer::GetServerConfig()
{
	return &m_srvConfig;
}

bool MainServer::Start()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pModel->Start()) return false;
	if (false == m_pController->Start()) return false;
	if (false == m_pMessage->Start()) return false;
	if (false == m_pCommunication->Start()) return false;
	return true;
}

void MainServer::Stop()
{
	m_pModel->Stop();
	m_pController->Stop();
	m_pCommunication->Stop();
	m_pMessage->Stop();
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
