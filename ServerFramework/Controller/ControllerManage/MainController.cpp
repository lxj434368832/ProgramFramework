#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"
#include "ControllerColleague.h"
#include "../../MainServer/IMainServer.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"
#include "../../Component/MessageHandle/IMessageHandle.h"
#include "../../Communication/ICommunication.h"
#include "../../Communication/TCPServer.h"
#include "../../3rdParty/Protobuf/include/Message.pb.h"
#include <memory>
#include <vector>

MainController::MainController(IControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	m_pCmmnt->RegisterMessageHandle(pbmsg::ELoginRq,
		std::bind(&MainController::HandleLoginRq, this, std::placeholders::_1, std::placeholders::_2));
	m_pCmmnt->RegisterMessageHandle(pbmsg::EHeartbeatNt,
		std::bind(&MainController::HandleHeartbeatNt, this, std::placeholders::_1, std::placeholders::_2));
}

MainController::~MainController()
{
}

bool MainController::Initialize()
{
	m_pUserMng = m_pModelMng->GetUserInfoManage();
     if(nullptr == m_pUserMng)
     {
		 loge() << "获取主模型失败!";
         return false;
     }

	 m_pTcpSrv = m_pCmmnt->GetTCPServer();
	 if (nullptr == m_pTcpSrv)
	 {
		 loge() << "获取TCP服务失败!";
		 return false;
	 }

    return true;
}

void MainController::Uninitialize()
{
	m_pUserMng = nullptr;
	m_pTcpSrv = nullptr;
}

bool MainController::StartServer()
{
	if (false == m_pTcpSrv->StartServer()) return false;
	m_pHeartbeatThread = new std::thread(&MainController::HeartbeatHandle, this);

	return true;
}

void MainController::StopServer()
{
	::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();
	RELEASE(m_pHeartbeatThread);
	RELEASE_HANDLE(m_hHeartbeatEvent);

	m_pTcpSrv->StopServer();
}

void MainController::HeartbeatHandle()
{
	m_hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hHeartbeatEvent)
	{
		loge() << "创建心跳事件失败！";
		return;
	}

	DWORD dwHeartbeatTime = m_pMain->GetServerConfig()->uHeartbeatTime;

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

		std::vector<unsigned> userList = m_pUserMng->GetOfflineUserList();
		for (unsigned uUserKey : userList)
		{
			m_pTcpSrv->Disconnect(uUserKey);
		}
	}
}

void MainController::HandleLoginRq(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::shared_ptr<SLoginRq> pRq(static_cast<SLoginRq*>(pMsg));
	SUserInfo* pUser = m_pUserMng->GetClientUserInfo(uUserKey);
	if (nullptr == pUser)
	{
		LOGE("登录用户key:%d userId:%d userName:%s不存在。", uUserKey, pRq->uUserId, pRq->strUserName.data());
		return;
	}
	m_pUserMng->LockUserInfo(uUserKey);
	pUser->uHeartCount = 0;
	pUser->uUserId = pRq->uUserId;
	pUser->strLoginName = pRq->strUserName;
	pUser->strPassword = pRq->strPassword;
	m_pUserMng->UnlockUserInfo(uUserKey);
	LOGM("用户key:%d id:%d loginName:%s 登录成功。",
		uUserKey, pUser->uUserId, pUser->strLoginName.data());

	m_pUserMng->AddUserIdUserKeyMap(pRq->uUserId, uUserKey);
	SLoginRs loginRs;
	loginRs.uResult = 0;
	loginRs.strMsg = "用户登录成功。";
	m_pTcpSrv->SendData(uUserKey, loginRs.SerializeAsPbMsg());
}

void MainController::HandleHeartbeatNt(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::shared_ptr<SHeartBeatNt> pRq(static_cast<SHeartBeatNt*>(pMsg));
	SUserInfo *pUser = m_pUserMng->GetClientUserInfo(uUserKey);
	if (nullptr == pUser)
	{
		loge() << "不存在用户uUserKey：" << uUserKey;
		return;
	}

	logm() << "接收到用户：" << uUserKey << "的心跳消息。";
	m_pUserMng->LockUserInfo(uUserKey);
	pUser->uHeartCount = 0;
	m_pUserMng->UnlockUserInfo(uUserKey);
}
