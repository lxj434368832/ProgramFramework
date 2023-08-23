#include "MainClient.h"
#include "..\View\ViewManage\IViewManage.h"
#include "..\Controller\ControllerManage\include\IControllerManage.h"
#include "..\Model\ModelManage\include\IModelManage.h"
#include "../CommonFile/CommonDefine.h"
#include "../3rdParty/MConfigManage/include/MConfigManage.h"
#include <QApplication>
#include <QDir>

MainClient::MainClient()
{
	m_pCfgMng = new MConfigManage(QApplication::applicationDirPath() + 
		"\\Config\\ClientConfig.ini");
	m_pModel = IModelManage::Create(this);
	m_pController = IControllerManage::Create(this);
	m_pView = IViewManage::Create(this);
}

MainClient::~MainClient()
{
	IViewManage::Delete(m_pView);
	IControllerManage::Delete(m_pController);
	IModelManage::Delete(m_pModel);
	RELEASE(m_pCfgMng);
}

ClientConfig * MainClient::GetClientConfig()
{
	return &m_clConfig;
}

MConfigManage* MainClient::GetConfigManage()
{
	return m_pCfgMng;
}

IViewManage * MainClient::GetViewManage()
{
	return m_pView;
}

IControllerManage * MainClient::GetControllerManage()
{
	return m_pController;
}

IModelManage * MainClient::GetModelManage()
{
	return m_pModel;
}

bool MainClient::StartClient()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pModel->Initialize()) return false;
	if (false == m_pController->Initialize()) return false;
	if (false == m_pView->Initialize()) return false;

	if (false == m_pView->StartClient()) return false;

	return true;
}

void MainClient::StopClient()
{
	m_pView->Uninitialize();
	m_pController->Uninitialize();
	m_pModel->Uninitialize();
}

bool MainClient::ReadConfigFile()
{
	m_clConfig.uMessageThreadCount = m_pCfgMng->value("Client", "MessageThreadCount", "0").toUInt();
	m_clConfig.uServerThreadCount = m_pCfgMng->value("Client", "IOCPThreadCount", "0").toUInt();
	m_clConfig.uHeartbeatTime = m_pCfgMng->value("Client", "HeartbeatTime", "0").toUInt();
	m_clConfig.strServerIP = m_pCfgMng->value("Connect", "ServerIP", "").toString().toLocal8Bit().data();
	m_clConfig.usServerPort = m_pCfgMng->value("Connect", "ServerPort", "0").toUInt();

	if (false == m_clConfig.CheckValid())
	{
		loge() << "读取配置文件发生错误，请检查配置文件！";
		return false;
	}

	return true;
}
