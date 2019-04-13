#include "ControllerManage.h"
#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"


ControllerManage::ControllerManage(IMainServer *_main) :
    IControllerManage(_main)
{
    m_mainController = new MainController(m_main);
}

ControllerManage::~ControllerManage()
{
    RELEASE(m_mainController);
}

MainController *ControllerManage::GetMainController()
{
    return m_mainController;
}

bool ControllerManage::Start()
{
    if(false == m_mainController->Start()) return false;

    return true;
}

void ControllerManage::Stop()
{
    m_mainController->Stop();
}
