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
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1、登陆web
	emit signalLoginTipMsg("开始登陆web验证账户名和密码...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();

	SUserInfo user;
	user.m_strUserName = strUserName;
	user.m_strPassword = strPassword;
	user.m_strMac = strMac;
	if (false == m_pMainModel->UserLoginWeb(user))
	{
		emit signalLoginMessageNt(false, "登陆web失败！");
		return;
	}

	//2、获取配置信息
	signalLoginTipMsg("开始获取客户端配置信息...");
	if (false == m_pMainModel->GetConfigInfo())
	{
		loge() << "获取web服务器配置信息失败！";
		//emit signalLoginMessageNt(false, "获取web服务器配置信息失败！");
		//return; 失败了没关系，本地还有配置项
		}
	emit signalLoginMessageNt(true, "连接服务器成功！");
}

void MainController::slotExecuteSystem()
{
	//3、添加TCP连接
	emit signalSplashMessage("开始连接服务器...");
	int iUserType = 1;
	if (false == m_pMainModel->ConnectTcpServer())
	{
		emit signalSplashMessage("连接服务器失败！");
		emit signalShowMainWindow(iUserType);		//此处根据具体的业务来加
	}
	else
		emit signalShowMainWindow(iUserType);
	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
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

		QSet<unsigned> setSrvKey = m_pMainModel->GetLoginServerList();
		for (unsigned uSrvKey : setSrvKey)
		{
			//m_pSrvCnnt->SendData(uSrvKey, pbMsg);
		}
	}
}

void MainController::HandleTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("连接服务Key:%d成功！", uServerType);

		m_pMainModel->LoginTcpServer(uServerType);
	}
	else
	{
		loge() << "连接服务uServerType:" << uServerType << "失败！";
	}
}

void MainController::HandleTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("服务Key:%d的连接断开", uServerType);
}

void MainController::HandleLoginRs(const unsigned uUserKey, std::auto_ptr<SRespondMsg> pRs)
{
	if (0 == pRs->uResult)
	{
		LOGM("登录服务Key:%d 成功。", uUserKey);
		//if (EST_CMD_SERVER == uUserKey)
		//{
		//	int iUserType = 1;
		//	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
		//	emit signalShowMainWindow(iUserType);
		//}
	}
	else
		LOGE("登录服务Key:%d 失败！", uUserKey);
}
