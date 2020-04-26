#include "ControllerColleague.h"
#include "IControllerManage.h"
#include "../../MainServer/IMainServer.h"
#include "../../Component/MessageHandle/IMessageHandle.h"

ControllerColleague::ControllerColleague(IControllerManage *pCtrlMng)
{
	m_pMain = pCtrlMng->GetMainServer();
	m_pCtrlMng = pCtrlMng;
	m_pModelMng = m_pMain->GetModelManage();
	m_pCmmnt = m_pMain->GetCommunication();
}

ControllerColleague::~ControllerColleague()
{
    m_pMain = nullptr;
    m_pCtrlMng = nullptr;
	m_pModelMng = nullptr;
	m_pCmmnt = nullptr;
}
