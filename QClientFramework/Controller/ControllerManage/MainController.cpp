#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/IMessageHandle.h"
#include "../../Component/TCPCommunication/ICommunication.h"
#include "../../Component/TCPCommunication/ServerConnect.h"
#include "../../Component/Utility/NetworkHelp.h"
#include "../../MainClient/IMainClient.h"
#include <memory>

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//连接信号和槽
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));
    connect(m_pCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

	//开启异步线程
    moveToThread(&m_thread);
    m_thread.start();

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

bool MainController::Initialize()
{
    m_pMainModel = m_pModel->GetMainModel();
     if(nullptr == m_pMainModel)
     {
         loge()<< "获取主模型失败!";
         return false;
     }

	 m_pSrvCnnt = m_pCmmnt->GetServerConnect();
	 if (nullptr == m_pSrvCnnt)
	 {
		 loge() << "获取服务端连接失败!";
		 return false;
	 }

	 m_pMsgHandle->RegisterMessageHandle();
	 m_pHeartbeatThread = new std::thread(&MainController::HeartbeatHandle, this);

    return true;
}

void MainController::Uninitialize()
{
	::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();
	RELEASE(m_pHeartbeatThread);
	RELEASE_HANDLE(m_hHeartbeatEvent);

	disconnect(m_pCmmnt, SIGNAL(signalTcpConnectNotify(uint, bool)),this, SLOT(slotTcpConnectNotify(uint, bool)));
	disconnect(m_pCmmnt, SIGNAL(signalTcpDisconnectNotify(uint)),this, SLOT(slotTcpDisconnectNotify(uint)));

	m_pMainModel = nullptr;
	m_pSrvCnnt = nullptr;
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1、登陆web
	signalLoginTipMsg("开始登陆web验证账户名和密码...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();
	//ClientConfig *pCfg = m_pMain->GetClientConfig();
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
	if (false == m_pCmmnt->ConnectServer())
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
		LOGM("连接服务Key:%d成功！",uServerType);
		SUserInfo &user = m_pMainModel->GetUserInfo();

		SLoginRq msg;
		msg.uUserId = uServerType;
		msg.strUserName = user.m_strUserName + std::to_string(uServerType);
		msg.strPassword = user.m_strPassword;
		m_pSrvCnnt->SendData(uServerType, msg.SerializeAsPbMsg());
	}
	else
	{
		loge() << "连接服务uServerType:" << uServerType << "失败！";
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("服务Key:%d的连接断开",uServerType);
}

void MainController::HeartbeatHandle()
{
	m_hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hHeartbeatEvent)
	{
		loge() << "创建心跳事件失败！";
		return;
	}

	SHeartBeatNt msg;
	msg.uUserId = 1;
	SPbMsg pbMsg = msg.SerializeAsPbMsg();

	DWORD dwHeartbeatTime = m_pMain->GetClientConfig()->uHeartbeatTime;

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(m_hHeartbeatEvent, dwHeartbeatTime);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "正常退出心跳线程。";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "心跳线程发生错误：" << ::GetLastError();
			break;
		}

		QSet<unsigned> setSrvKey = m_pMainModel->GetLoginServer();
		for (unsigned uSrvKey : setSrvKey)
		{
			m_pSrvCnnt->SendData(uSrvKey, pbMsg);
		}
	}
}

void MainController::HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::unique_ptr<SRespondMsg> pRs(static_cast<SRespondMsg*>(pMsg));

	if (0 == pRs->uResult)
	{
		LOGM("登录服务Key%d成功。", uUserKey);
		m_pMainModel->AddLoginServer(uUserKey);

		if (EST_CMD_SERVER == uUserKey)
		{
			int iUserType = 1;
			//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
			emit signalShowMainWindow(iUserType);
		}
	}
	else
		LOGE("登录服务Key%d失败！", uUserKey);
}
