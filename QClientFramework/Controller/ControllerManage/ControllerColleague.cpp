#include "ControllerColleague.h"
#include "IControllerManage.h"
#include "../../MainClient/IMainClient.h"
#include "ControllerManage.h"
#include "../../Component/MessageHandle/IMessageHandle.h"

ControllerColleague::ControllerColleague(ControllerManage *pCtrlMng, QObject *parent)
    :QObject(parent)
{
    m_pMain = pCtrlMng->GetMainClient();
	m_pCtrlMng = pCtrlMng;
    m_pModel = m_pMain->GetModelManage();
}

ControllerColleague::~ControllerColleague()
{
    m_pMain = nullptr;
	m_pCtrlMng = nullptr;
    m_pModel = nullptr;
}
