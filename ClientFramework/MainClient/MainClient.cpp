#include "MainClient.h"
#include "..\ViewManage\MainView.h"
#include "..\LogicManage\MainLogic.h"
#include "..\ModelManage\MainModel.h"
#include "..\modules\MessageModule\MessageModule.h"
#include "..\include\CommonDefine.h"
#include "..\..\Framework\include\Utils.h"
#include "..\..\Framework\include\cfg_reg_reader.h"
#include "..\modules\CommunicationModule\MainCommunication.h"

MainClient::MainClient()
{
#ifdef ZX_LOGGING_H_
	zxl::zx_logger *logger = zxl::zx_logger::instance();
	zxl::configure::add_flag(zxl::config_flag::rolling_by_day);		//dgx：设置按时间打印日志
	zxl::configure tmp_cfg(zxl::helpers::get_module_dir() + "\\config\\LogConfig.ini");
	logger->reconfiguration(tmp_cfg);
#endif

	m_pView = new MainView(this);
	m_pLogic = new MainLogic(this);
	m_pModel = new MainModel(this);

	m_pCommunication = new MainCommunication(this);
	m_pMessage = new MessageModule(this);
}

MainClient::~MainClient()
{
	RELEASE(m_pView);
	RELEASE(m_pLogic);
	RELEASE(m_pModel);
	RELEASE(m_pCommunication);
	RELEASE(m_pMessage);
	zxl::zx_logger::delete_instance();
}

ClientConfig * MainClient::GetClientConfig()
{
	return &m_clConfig;
}

IViewInterface * MainClient::GetViewInterface()
{
	return m_pView;
}

ILogicInterface * MainClient::GetLogicInterface()
{
	return m_pLogic;
}

IModelInterface * MainClient::GetModelInterface()
{
	return m_pModel;
}

ICommunication * MainClient::GetMainCommunication()
{
	return m_pCommunication;
}

IMessage * MainClient::GetMessageModule()
{
	return m_pMessage;
}

bool MainClient::Start()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pView->Start()) return false;
	if (false == m_pLogic->Start()) return false;
	if (false == m_pModel->Start()) return false;
	if (false == m_pCommunication->Start()) return false;
	if (false == m_pMessage->Start()) return false;

	return true;
}

void MainClient::Stop()
{
	m_pView->Stop();
	m_pLogic->Stop();
	m_pModel->Stop();
	m_pCommunication->Stop();
	m_pMessage->Stop();
}

bool MainClient::ReadConfigFile()
{
	std::string strConfigPath = Utils::get_module_path(nullptr, "\\config\\ClientConfig.ini");
	cfg_reg_reader cfg_reader;
	if (!cfg_reader.read_from_cfg(strConfigPath))
	{
		loge() << "读取配置文件:" << strConfigPath.c_str() << "失败！";
		return false;
	}
	m_clConfig.uMessageThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Client", "MessageThreadCount", "0"));
	m_clConfig.uIOCPThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Client", "IOCPThreadCount", "0"));
	m_clConfig.uHeartbeatTime = std::stoul(cfg_reader.get_value_from_cfg("Client", "HeartbeatTime", "0"));
	m_clConfig.strServerIP = cfg_reader.get_value_from_cfg("Connect", "ServerIP", "");
	m_clConfig.usServerPort = std::stoi(cfg_reader.get_value_from_cfg("Connect", "ServerPort", "0"));

	if (false == m_clConfig.CheckValid())
	{
		loge() << "读取配置文件发生错误，请检查配置文件！";
		return false;
	}
	return true;
}
