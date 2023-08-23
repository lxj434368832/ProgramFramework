#include "ControllerManage.h"
#include "MainController.h"
#include "../../../MainClient/IMainClient.h"
#include "../../../CommonFile/CommonDefine.h"
#include "../../../Model/ModelManage/include/IModelManage.h"


IControllerManage* IControllerManage::Create(IMainClient *pMain)
{
	return new ControllerManage(pMain);
}

void IControllerManage::Delete(IControllerManage*& p)
{
	RELEASE(p);
}


ControllerManage::ControllerManage(IMainClient *_main) :
    IControllerManage(_main)
{
    m_pMainCtrl = new MainController(this);
}

ControllerManage::~ControllerManage()
{
	RELEASE(m_pMainCtrl);
}

bool ControllerManage::Initialize()
{
	if (false == m_pMainCtrl->Initialize()) return false;

	return true;
}

void ControllerManage::Uninitialize()
{
	m_pMainCtrl->Uninitialize();
}

IMainController *ControllerManage::GetMainController()
{
    return m_pMainCtrl;
}
