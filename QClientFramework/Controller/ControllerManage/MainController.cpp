#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/HandleRequestMessage.h"
#include "../../Component/TCPCommunication/ITCPCommunication.h"

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//连接信号和槽
    connect(m_pTcpCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pTcpCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

    moveToThread(&m_thread);
    m_thread.start();
}

MainController::~MainController()
{
	m_thread.quit();
	if (false == m_thread.wait(3000))
	{
		loge() << "主控制器线程退出等待超时！";
		m_thread.terminate();
	}

	m_pMainModel = nullptr;
}

bool MainController::Start()
{
    m_pMainModel = m_pModel->GetMainModel();
     if(nullptr == m_pMainModel)
     {
         loge()<< "获取主模型失败!";
         return false;
     }

    return true;
}

void MainController::Stop()
{
}

void MainController::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("连接服务:%d成功！",uServerType);
		m_pMainModel->AddUser(uServerType);
		std::string strMsg = m_pHandleRqMsg->BuildLoginRequest("mingqiaowen", "123456");
		m_pTcpCmmnt->SendData(uServerType, strMsg.data(), strMsg.length());
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("userKey:%d连接断开",uServerType);
	m_pMainModel->DeleteUser(uServerType);
}

void MainController::UserLoginResult(unsigned uUserKey, bool bRet, std::string strMsg)
{
	LOGM("用户:%d 登录结果：%2s", uUserKey, strMsg.c_str());
}
