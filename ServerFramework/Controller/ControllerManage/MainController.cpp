#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"
#include "ControllerColleague.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"
#include "../../Component/MessageHandle/HandleRespondMessage.h"
#include "../../Component/TCPCommunication/ITCPCommunication.h"

MainController::MainController(IControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	m_pUserMng = nullptr;
}

MainController::~MainController()
{
}

bool MainController::Start()
{
	m_pUserMng = m_pModelMng->GetUserInfoManage();
     if(nullptr == m_pUserMng)
     {
		 loge() << "获取主模型失败!";
         return false;
     }

    return true;
}

void MainController::Stop()
{
	m_pUserMng = nullptr;
}

void MainController::UserLogin(ClientUserInfo &userInfo)
{
	logm() << "收到用户登录消息name:" << userInfo.m_strName << " password:" << userInfo.m_strPassword;
	m_pUserMng->SetUserInfo(userInfo.m_uUserKey, userInfo);
	std::string strMsg = m_pHandleRsMsg->BuildLoginRespond(true, "用户登录成功");
	m_pTcpCmmnt->SendData(userInfo.m_uUserKey, strMsg.data(), strMsg.length());
}
