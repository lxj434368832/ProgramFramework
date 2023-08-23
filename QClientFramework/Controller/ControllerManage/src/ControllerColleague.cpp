#include "ControllerColleague.h"
#include "../include/IControllerManage.h"
#include "../../../MainClient/IMainClient.h"
#include "ControllerManage.h"

ControllerColleague::ControllerColleague(IControllerManage *pCtrlMng)
{
    m_pMain = pCtrlMng->GetMainClient();
	m_pCtrlMng = pCtrlMng;
	m_pModelMng = m_pMain->GetModelManage();
}

ControllerColleague::~ControllerColleague()
{
    m_pMain = nullptr;
	m_pCtrlMng = nullptr;
	m_pModelMng = nullptr;
}
