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

bool ControllerManage::Initialize()
{
    if(false == m_pMainCtrl->Initialize()) return false;

    return true;
}

void ControllerManage::Uninitialize()
{
	m_pMainCtrl->Uninitialize();
}
