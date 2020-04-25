#include "MainModel.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\..\Controller\ControllerManage\IControllerManage.h"
#include "..\..\Controller\ControllerManage\MainController.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "../../Component/TCPCommunication/Communication.h"
#include "../../Component/TCPCommunication/ServerConnect.h"
#include "..\..\Component\MessageHandle\MessageHandle.h"
#include "..\..\Component\MessageHandle\PbMessageHandle.h"
#include "..\..\3rdParty\Protobuf\include\Message.pb.h"
#include <memory>

MainModel::MainModel(IModelManage* pMdlMng) :
	ModelColleague(pMdlMng)
{
	m_pSrvCnnt = m_pCmmnt->GetServerConnect();
	connect(m_pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool)), SLOT(slotTcpConnectNotify(uint, bool)));
	connect(m_pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint)), SLOT(slotTcpDisconnectNotify(uint)));

}

MainModel::~MainModel()
{
	disconnect(m_pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool)), this, SLOT(slotTcpConnectNotify(uint, bool)));
	disconnect(m_pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint)), this, SLOT(slotTcpDisconnectNotify(uint)));
	m_pSrvCnnt = nullptr;
}

bool MainModel::Initialize()
{
	PbMessageHandle *pPbMsgHdl = m_pMsgHandle->GetPbMessageHandle();
	pPbMsgHdl->RegisterMessageFunction(pbmsg::ELoginRs, std::bind(&MainModel::OnLoginRs, this, std::placeholders::_1, std::placeholders::_2));

	m_pMainCtrl = m_pMain->GetControllerManage()->GetMainController();
	if (nullptr == m_pMainCtrl)
	{
		loge() << "获取主控制器失败!";
		return false;
	}
    return true;
}

void MainModel::Uninitialize()
{
	PbMessageHandle *pPbMsgHdl = m_pMsgHandle->GetPbMessageHandle();
	pPbMsgHdl->RemoveMessageFunction(pbmsg::ELoginRs);
	m_pMainCtrl = nullptr;
}

bool MainModel::UserLoginWeb(SUserInfo &user)
{
	user.m_uUserId = 1;
	//m_pWebCnnt->UserLogin(strUserName, strPassword, strMac);
	m_user = user;
	return 0 != user.m_uUserId;
}

bool MainModel::GetConfigInfo()
{
	ClientConfig *pCfg = m_pMain->GetClientConfig();
	//return m_pWebCnnt->GetConfigInfo(user.m_strMac, pCfg);
	return true;
}

bool MainModel::ConnectTcpServer()
{
	ClientConfig *cfg = m_pMain->GetClientConfig();

	for (int i = EST_CMD_SERVER; i < EST_SERVER_COUNT; i++)
	{
		if (false == m_pSrvCnnt->AddConnect(i, cfg->strServerIP, cfg->usServerPort, 1))
		{
			loge() << "添加对命令服务器的连接失败！";
			return false;
		}
	}
	return true;
}

void MainModel::LoginTcpServer(unsigned uServerType)
{
	SLoginRq msg;
	msg.uUserId = uServerType;
	msg.strUserName = m_user.m_strUserName + std::to_string(uServerType);
	msg.strPassword = m_user.m_strPassword;
	m_pSrvCnnt->SendData(uServerType, msg.SerializeAsPbMsg());
}

void MainModel::LockUserInfo()
{
	m_lckUserInfo.lock();
}

void MainModel::UnlockUserInfo()
{
	m_lckUserInfo.unlock();
}

void MainModel::AddLoginServer(UserKey uUserKey)
{
}

QSet<unsigned> MainModel::GetLoginServerList()
{
	return m_setSrvUser;
}

void MainModel::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	m_pMainCtrl->HandleTcpConnectNotify(uServerType, bSuccess);
}

void MainModel::slotTcpDisconnectNotify(unsigned uServerType)
{
	m_lckSrvUser.lock();
	m_setSrvUser.remove(uServerType);
	m_lckSrvUser.unlock();

}

void MainModel::OnLoginRs(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::auto_ptr<SRespondMsg> pRs(static_cast<SRespondMsg*>(pMsg));

	if (0 == pRs->uResult)
	{
		m_lckSrvUser.lock();
		m_setSrvUser.insert(uUserKey);
		m_lckSrvUser.unlock();
	}

	m_pMainCtrl->HandleLoginRs(uUserKey, pRs);
}

SUserInfo& MainModel::GetUserInfo()
{
	return m_user;
}
