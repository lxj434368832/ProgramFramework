#include "MainClient.h"
#include "..\View\ViewManage\ViewManage.h"
#include "..\Controller\ControllerManage\ControllerManage.h"
#include "..\Model\ModelManage\ModelManage.h"
#include "..\Component\TCPCommunication\Communication.h"
#include "..\Component\MessageHandle\MessageHandle.h"
#include "../CommonFile/CommonDefine.h"
#include "../3rdParty/MConfigManage/include/MConfigManage.h"
#include <QApplication>
#include <QDir>
#include <stdio.h>

MainClient::MainClient()
{
	m_pCfgMng = new MConfigManage(QApplication::applicationDirPath() + 
		"\\Config\\ClientConfig.ini");
	m_pTCPCommunication = new Communication(this);
	m_pMessage = new MessageHandle(this);
	m_pModel = new ModelManage(this);
	m_pController = new ControllerManage(this);
	m_pView = new ViewManage(this);
}

MainClient::~MainClient()
{
	RELEASE(m_pView);
	RELEASE(m_pController);
	RELEASE(m_pModel);
	RELEASE(m_pMessage);
	RELEASE(m_pTCPCommunication);
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

ICommunication * MainClient::GetCommunication()
{
	return m_pTCPCommunication;
}

IMessageHandle * MainClient::GetMessageHandle()
{
	return m_pMessage;
}

bool MainClient::StartClient()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pTCPCommunication->Initialize()) return false;
	if (false == m_pMessage->Initialize()) return false;
	if (false == m_pModel->Initialize()) return false;
	if (false == m_pController->Initialize()) return false;
	if (false == m_pView->Initialize()) return false;

	if (false == m_pView->LoginWebServer()) return false;
	m_pController->ExecuteSystem();

	return true;
}

void MainClient::StopClient()
{
	m_pView->Uninitialize();
	m_pController->Uninitialize();
	m_pModel->Uninitialize();
	m_pMessage->Uninitialize();
	m_pTCPCommunication->Uninitialize();
}

bool MainClient::ReadConfigFile()
{
	m_clConfig.uMessageThreadCount = m_pCfgMng->value("Client", "MessageThreadCount", "0").toUInt();
	m_clConfig.uIOCPThreadCount = m_pCfgMng->value("Client", "IOCPThreadCount", "0").toUInt();
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
