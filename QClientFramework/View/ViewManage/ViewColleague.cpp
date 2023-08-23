#include "ViewColleague.h"
#include "ViewMediator.h"
#include "../../MainClient/IMainClient.h"
#include "../../Controller/ControllerManage/include/IControllerManage.h"

ViewColleague::ViewColleague(ViewMediator* mdt){
    m_pMdt = mdt;
    m_pMain = mdt->GetMainClient();
    m_pCtrlMng = m_pMain->GetControllerManage();
}

ViewColleague::~ViewColleague()
{
    m_pMdt = nullptr;
    m_pMain = nullptr;
	m_pCtrlMng = nullptr;
}

