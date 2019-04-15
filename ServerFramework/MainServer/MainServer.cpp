#include "MainServer.h"
#include "../../Framework/include/log/logging.h"
#include "../../Framework/include/cfg_reg_reader.h"
#include "../../Framework/include/Utils.h"
#include "../CommonFile//CommonDefine.h"
#include "../Model/ModelManage/ModelManage.h"
#include "../Controller/ControllerManage/ControllerManage.h"
#include "../Component/MessageHandle/MessageHandle.h"
#include "../Component/TCPCommunication/TCPCommunication.h"

MainServer::MainServer()
{
#ifdef ZX_LOGGING_H_
	zxl::zx_logger *logger = zxl::zx_logger::instance();
	zxl::configure::add_flag(zxl::config_flag::rolling_by_day);		//dgx：设置按时间打印日志
	zxl::configure tmp_cfg(zxl::helpers::get_module_dir() + "\\config\\LogConfig.ini");
	logger->reconfiguration(tmp_cfg);
#endif

	m_pModel = new ModelManage(this);
	m_pController = new ControllerManage(this);
	m_pMessage = new MessageHandle(this);
	m_pCommunication = new TCPCommunication(this);
}

MainServer::~MainServer()
{
	RELEASE(m_pCommunication);
	RELEASE(m_pMessage);
	RELEASE(m_pController);
	RELEASE(m_pModel);
	zxl::zx_logger::delete_instance();
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
	cfg_reg_reader cfg_reader;
	if (!cfg_reader.read_from_cfg(strConfigPath))
	{
		loge() << "读取配置文件:" << strConfigPath.c_str() << "失败！";
		return false;
	}
	m_srvConfig.usListenPort	= std::stoi(cfg_reader.get_value_from_cfg("Server", "ListenPort", "0"));
	m_srvConfig.uServerThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "ServerThreadCount", "0"));
	m_srvConfig.uMessageThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "MessageThreadCount", "0"));
	m_srvConfig.uInitAcceptCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "InitAcceptCount", "0"));
	m_srvConfig.uHeartbeatTime = std::stoul(cfg_reader.get_value_from_cfg("Server", "HeartbeatTime", "0"));

	if (false == m_srvConfig.CheckValid())
	{
		loge() << "读取配置文件发生错误，请检查配置文件！";
		return false;
	}
	return true;
}
