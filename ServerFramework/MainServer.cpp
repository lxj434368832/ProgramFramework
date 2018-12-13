#include "stdafx.h"
#include "MainServer.h"
#include "./Framework/zxu_utils.h"
#include "IOCPServer.h"
#include "INetInterface.h"

MainServer::MainServer()
{
#ifdef ZX_LOGGING_H_
	zxl::zx_logger *logger = zxl::zx_logger::instance();
	zxl::configure::add_flag(zxl::config_flag::rolling_by_day);		//dgx：设置按时间打印日志
	zxl::configure tmp_cfg(zxl::helpers::get_module_dir() + "\\config\\LogConfig.ini");
	logger->reconfiguration(tmp_cfg);
#endif
	m_pClientList = new INetInterface;
	m_pIOCPServer = new IOCPServier(m_pClientList);
}

MainServer::~MainServer()
{
	RELEASE(m_pIOCPServer);
	RELEASE(m_pClientList);

	zxl::zx_logger::delete_instance();
}

bool MainServer::Start()
{
	if (false == ReadConfigFile()) return true;
	if (false == m_pIOCPServer->StartServer(m_srvConfig.usListenPort,
		m_srvConfig.uInitAcceptCount, m_srvConfig.uServerThreadCount))
		return false;
}

bool MainServer::Stop()
{
	m_pIOCPServer->StopServer();
	return true;
}

bool MainServer::ReadConfigFile()
{
	std::string strConfigPath = zxu::get_module_path(zxu::get_self_module_handle(), "\\config\\ServerConfig.ini");
	zxu::cfg_reg_reader cfg_reader;
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
	m_srvConfig.strServerIP = cfg_reader.get_value_from_cfg("Connect", "ServerIP", "");
	m_srvConfig.usServerPort = std::stoi(cfg_reader.get_value_from_cfg("Connect", "ServerPort", "0"));

	if (false == m_srvConfig.CheckValid())
	{
		loge() << "读取配置文件发生错误，请检查配置文件！";
		return false;
	}
	return true;
}
