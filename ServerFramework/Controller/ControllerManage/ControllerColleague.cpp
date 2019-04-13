#include "ControllerColleague.h"
#include "IControllerManage.h"
#include "../../MainServer/IMainServer.h"

ControllerColleague::ControllerColleague(IMainServer *main)
{
    m_main = main;
    //m_controller = main->GetControllerInterface();
    //m_model = main->GetModelInterface();
}

ControllerColleague::~ControllerColleague()
{
    m_main = nullptr;
    m_controller = nullptr;
    m_model = nullptr;
}
