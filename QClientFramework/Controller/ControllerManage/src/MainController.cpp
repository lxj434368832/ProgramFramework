#include "MainController.h"
#include "../../../Model/ModelManage/include/IModelManage.h"
#include "../../../CommonFile/CommonDefine.h"
#include "../../../Model/MessageHandle/include/IMessageHandle.h"
#include "../../../Model/Communication/include/IServerConnect.h"
#include "../..\..\3rdParty\Protobuf\include\Message.pb.h"
#include "../../../Component/Utility/NetworkHelp.h"
#include "../../../MainClient/IMainClient.h"
#include <memory>

IMainController* IMainController::Create(IControllerManage* pCtrlMng)
{
	return new MainController(pCtrlMng);
}

void IMainController::Delete(IMainController*& pCtrlMng)
{
	RELEASE(pCtrlMng);
}

MainController::MainController(IControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//连接信号和槽
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));

	qRegisterMetaType<std::string>("std::string");
}

MainController::~MainController()
{
}

bool MainController::Initialize()
{
	
	if (IMessageHandle *pMsgHandle = m_pModelMng->GetPbMessageHandle())
	{
		pMsgHandle->RegisterMsgFunction(pbmsg::ELoginRs, std::bind(&MainController::HandleLoginRs, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (IServerConnect *pSrvCnnt = m_pModelMng->GetServerConnect())
	{
		connect(pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool)), SLOT(slotTcpConnectNotify(uint, bool)));
		connect(pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint)), SLOT(slotTcpDisconnectNotify(uint)));
	}

	//开启异步线程
	moveToThread(&m_thread);
	m_thread.start();

	 m_pHeartbeatThread = new std::thread(&MainController::HandleHeartbeat, this);

    return true;
}

void MainController::Uninitialize()
{
	if (m_hHeartbeatEvent)
		::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();
	RELEASE(m_pHeartbeatThread);
	RELEASE_HANDLE(m_hHeartbeatEvent);

	m_thread.quit();
	if (false == m_thread.wait(3000))
	{
		loge() << "主控制器线程退出等待超时！";
		m_thread.terminate();
	}

	if (IServerConnect *pSrvCnnt = m_pModelMng->GetServerConnect())
	{
		disconnect(pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool))
			, this, SLOT(slotTcpConnectNotify(uint, bool)));
		disconnect(pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint))
			, this, SLOT(slotTcpDisconnectNotify(uint)));
	}
	if (IMessageHandle *pMsgHandle = m_pModelMng->GetPbMessageHandle())
	{
		pMsgHandle->RemoveMsgFunction(pbmsg::ELoginRs);
	}
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1、登陆web
	emit signalLoginTipMsg("开始登陆web验证账户名和密码...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();
	//m_pWebCnnt->UserLogin(strUserName, strPassword, strMac);

	m_user.m_uUserId = 1;
	m_user.m_strUserName = strUserName;
	m_user.m_strPassword = strPassword;
	m_user.m_strMac = strMac;
	if (0 == m_user.m_uUserId)
	{
		emit signalLoginMessageNt(false, "登陆web失败！");
		return;
	}

	//2、从web获取配置信息
	signalLoginTipMsg("开始获取客户端配置信息...");
	ClientConfig* pCfg = m_pMain->GetClientConfig();
	/*if (false == m_pWebCnnt->GetConfigInfo(user.m_strMac, pCfg))
	{
		loge() << "获取web服务器配置信息失败！";
		//emit signalLoginMessageNt(false, "获取web服务器配置信息失败！");
		//return; 失败了没关系，本地还有配置项
		}*/
	emit signalLoginMessageNt(true, "连接服务器成功！");
}

void MainController::slotExecuteSystem()
{
	//3、添加TCP连接
	emit signalSplashMessage("开始连接服务器...");
	int iUserType = 1;
	ClientConfig* cfg = m_pMain->GetClientConfig();

	for (int i = EST_CMD_SERVER; i < EST_SERVER_COUNT; i++)
	{
		IServerConnect *pSrvCnnt = m_pModelMng->GetServerConnect();
		if (!pSrvCnnt) continue;
		if (false == pSrvCnnt->AddConnect(i, cfg->strServerIP, cfg->usServerPort, 1))
		{
			loge() << "添加对命令服务器的连接失败！";
			emit signalSplashMessage("连接服务器失败！");
			emit signalShowMainWindow(iUserType);		//此处根据具体的业务来加
			return;
		}
	}
	emit signalShowMainWindow(iUserType);
}

void MainController::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("连接服务Key:%d成功！", uServerType);
		m_lckSrvUser.lock();
		m_setSrvUser.insert(uServerType);
		m_lckSrvUser.unlock();

		SLoginRq msg;
		msg.uUserId = uServerType;
		msg.strUserName = m_user.m_strUserName + std::to_string(uServerType);
		msg.strPassword = m_user.m_strPassword;
		if (IServerConnect *pSrvCnnt = m_pModelMng->GetServerConnect())
			pSrvCnnt->SendData(uServerType, msg.SerializeAsPbMsg());
	}
	else
	{
		loge() << "连接服务uServerType:" << uServerType << "失败！";
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	m_lckSrvUser.lock();
	m_setSrvUser.remove(uServerType);
	m_lckSrvUser.unlock();
	LOGM("服务Key:%d的连接断开", uServerType);
}

void MainController::HandleHeartbeat()
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

		for (unsigned uSrvKey : m_setSrvUser)
		{
			if (IServerConnect *pSrvCnnt = m_pModelMng->GetServerConnect())
				pSrvCnnt->SendData(uSrvKey, pbMsg);
		}
	}
}

void MainController::HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg)
{
	SRespondMsg* pRs = dynamic_cast<SRespondMsg*>(pMsg);

	if (0 == pRs->uResult)
	{
		LOGM("登录服务Key:%d 成功。", uUserKey);
		//if (EST_CMD_SERVER == uUserKey)
		//{
		//	emit signalShowMainWindow(iUserType);
		//}
	}
	else
		LOGE("登录服务Key:%d 失败！", uUserKey);
}
