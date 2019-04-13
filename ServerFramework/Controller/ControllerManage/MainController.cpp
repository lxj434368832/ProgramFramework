#include "MainController.h"
//#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"

MainController::MainController(IMainServer *main) :
    ControllerColleague(main)
{
}

MainController::~MainController()
{
}

bool MainController::Start()
{
    //m_mainModel = m_model->GetMainModel();
    // if(nullptr == m_mainModel)
    // {
    //     loge()<<QStringLiteral("获取主模型失败!");
    //     return false;
    // }

    return true;
}

void MainController::Stop()
{
    //m_mainModel = nullptr;
}
