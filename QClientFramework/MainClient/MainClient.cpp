#include "MainClient.h"
#include "..\View\ViewManage\ViewManage.h"
#include "..\Controller\ControllerManage\ControllerManage.h"
#include "..\Model\ModelManage\ModelManage.h"
#include "..\Component\TCPCommunication\TCPCommunication.h"
//#include "..\Component\MessageModule\MessageModule.h"
#include "../CommonFile/CommonDefine.h"
#include "../3rdParty/MConfigManage/include/MConfigManage.h"
#include <QApplication>
#include <QDir>
#include <stdio.h>

MainClient::MainClient()
{
    m_pController = new ControllerManage(this);
    m_pModel = new ModelManage(this);
    m_pCommunication = new TCPCommunication(this);
	//	m_pMessage = new MessageModule(this);

    m_pView = new ViewManage(this);
}

MainClient::~MainClient()
{
	RELEASE(m_pView);
    RELEASE(m_pController);
	RELEASE(m_pModel);
	RELEASE(m_pCommunication);
//    RELEASE(m_pMessage);
}

ClientConfig * MainClient::GetClientConfig()
{
	return &m_clConfig;
}

IViewManage * MainClient::GetViewInterface()
{
	return m_pView;
}

IControllerManage * MainClient::GetLogicInterface()
{
    return m_pController;
}

IModelManage * MainClient::GetModelInterface()
{
	return m_pModel;
}

ITCPCommunication * MainClient::GetMainCommunication()
{
	return m_pCommunication;
}

IMessage * MainClient::GetMessageModule()
{
	return m_pMessage;
}

bool MainClient::Start()
{
//	if (false == ReadConfigFile()) return false;
    if (false == m_pController->Start()) return false;
	if (false == m_pModel->Start()) return false;
	if (false == m_pCommunication->Start()) return false;
//	if (false == m_pMessage->Start()) return false;

	if (false == m_pView->Start()) return false;
	return true;
}

void MainClient::Stop()
{
	m_pView->Stop();
    m_pController->Stop();
	m_pModel->Stop();
	m_pCommunication->Stop();
//	m_pMessage->Stop();
}

bool MainClient::ReadConfigFile()
{
    MConfigManage config(QApplication::applicationDirPath() +"\\config\\ClientConfig.ini" );
    m_clConfig.uMessageThreadCount = config.value("Client", "MessageThreadCount", "0").toUInt();
    m_clConfig.uIOCPThreadCount =  config.value("Client", "IOCPThreadCount", "0").toUInt();
    m_clConfig.uHeartbeatTime = config.value("Client", "HeartbeatTime", "0").toUInt();
    m_clConfig.strServerIP =  config.value("Connect", "ServerIP", "").toString().toLocal8Bit().data();
    m_clConfig.usServerPort =  config.value("Connect", "ServerPort", "0").toUInt();

    if (false == m_clConfig.CheckValid())
    {
        loge() << QStringLiteral("读取配置文件发生错误，请检查配置文件！");
        //return false;
    }

	return true;
}
