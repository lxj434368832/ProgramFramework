#include "ControllerManage.h"
#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/ModelManage.h"
#include "../../MainClient/IMainClient.h"


ControllerManage::ControllerManage(IMainClient *_main) :
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
	ClientConfig *cfg = m_pMain->GetClientConfig();
	if (nullptr == cfg)
	{
		loge() << "ÅäÖÃÏîÎª¿Õ£¡";
		return false;
	}

    if(false == m_pMainCtrl->Initialize()) return false;

    return true;
}

void ControllerManage::Uninitialize()
{
    m_pMainCtrl->Uninitialize();
}
