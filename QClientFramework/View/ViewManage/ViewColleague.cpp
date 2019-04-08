#include "ViewColleague.h"
#include "ViewMediator.h"
#include "../../MainClient/IMainClient.h"
#include "../../Controller/ControllerManage/IControllerManage.h"

ViewColleague::ViewColleague(ViewMediator* mdt){
    m_mediator = mdt;
    m_mainClient = mdt->GetMainClient();
    m_controller = m_mainClient->GetControllerInterface();
}

ViewColleague::~ViewColleague()
{
    m_mediator = nullptr;
    m_mainClient = nullptr;
	m_controller = nullptr;
}

