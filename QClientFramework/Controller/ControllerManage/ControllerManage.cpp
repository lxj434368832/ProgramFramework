#include "ControllerManage.h"
#include "MainController.h"
#include "StatisticController.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/ModelManage.h"


ControllerManage::ControllerManage(IMainClient *_main) :
	QObject(),
    IControllerManage(_main)
{
    m_mainController = new MainController(m_main);
    m_statisticController = new StatisticController(m_main);
}

ControllerManage::~ControllerManage()
{
    RELEASE(m_mainController);
    RELEASE(m_statisticController);
}

MainController *ControllerManage::GetMainController()
{
    return m_mainController;
}

StatisticController *ControllerManage::GetStatisticController()
{
    return m_statisticController;
}

bool ControllerManage::Start()
{
    if(false == m_mainController->Start()) return false;
    if(false == m_statisticController->Start()) return false;

    return true;
}

void ControllerManage::Stop()
{
    m_mainController->Stop();
    m_statisticController->Stop();
}
