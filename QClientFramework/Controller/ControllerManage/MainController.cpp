#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/HandleRequestMessage.h"
#include "../../Component/TCPCommunication/ITCPCommunication.h"
#include "../../Component/Utility/NetworkHelp.h"
#include "../../MainClient/IMainClient.h"

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//连接信号和槽
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));
    connect(m_pTcpCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pTcpCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

	//开启异步线程
    moveToThread(&m_thread);
    m_thread.start();


	//注册通讯消息处理
	//m_pTcpCmmnt->RegistMessageHandle(EMsgType::e_user_login_rs, std::bind(&MainController::HandlTcpLoginRespondMsg,
	//	this, std::placeholders::_1, std::placeholders::_2));

	qRegisterMetaType<std::string>("std::string");
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

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1、登陆web
	signalLoginTipMsg("开始登陆web验证账户名和密码...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();
	ClientConfig *pCfg = m_pMain->GetClientConfig();
	//SUserInfo user = m_pWebCnnt->UserLogin(strUserName, strPassword, strMac);

	SUserInfo user;
	user.m_uUserId = 1;
	user.m_strUserName = strUserName;
	user.m_strPassword = strPassword;

	if (0 == user.m_uUserId)
	{
		emit signalLoginMessageNt(false, "登陆web失败！");
		return;
	}
	user.m_strMac = strMac;
	m_pMainModel->SaveUserInfo(user);

	/*//2、获取配置信息
	signalLoginTipMsg("开始获取客户端配置信息...");
	if (false == m_pWebCnnt->GetConfigInfo(strMac, pCfg))
	{
		loge() << "获取web服务器配置信息失败！";
		//emit signalLoginMessageNt(false, "获取web服务器配置信息失败！");
		//return; 失败了没关系，本地还有配置项
	}*/

	//3、添加TCP连接
	emit signalLoginTipMsg("开始连接服务器...");
	if (false == m_pTcpCmmnt->ConnectServer())
	{
		emit signalLoginMessageNt(false, "连接服务器失败！");
	}
	else
		emit signalLoginMessageNt(true, "连接服务器成功！");
}

void MainController::slotExecuteSystem()
{
	int iUserType = 1;
	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
	emit signalShowMainWindow(iUserType);
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
