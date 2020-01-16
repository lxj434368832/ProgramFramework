#include "ControllerManage.h"
#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"


ControllerManage::ControllerManage(IMainServer *_main) :
    IControllerManage(_main)
{
    m_pMainCtrl = new MainController(this);
}

ControllerManage::~ControllerManage()
{
    RELEASE(m_pMainCtrl);
}

MainController *ControllerManage::GetMainController()
{
    return m_pMainCtrl;
}

bool ControllerManage::Start()
{
    if(false == m_pMainCtrl->Start()) return false;

    return true;
}

void ControllerManage::Stop()
{
	m_pMainCtrl->Stop();
}
