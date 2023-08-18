#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/IMessageHandle.h"
#include "..\..\Component\MessageHandle\PbMessageHandle.h"
#include "..\..\3rdParty\Protobuf\include\Message.pb.h"
#include "../../Component/Communication/ICommunication.h"
#include "../../Component/Communication/ServerConnect.h"
#include "../../Component/Utility/NetworkHelp.h"
#include "../../MainClient/IMainClient.h"
#include <memory>

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//�����źźͲ�
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));

	m_pSrvCnnt = m_pCmmnt->GetServerConnect();
	connect(m_pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool)), SLOT(slotTcpConnectNotify(uint, bool)));
	connect(m_pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint)), SLOT(slotTcpDisconnectNotify(uint)));

	//�����첽�߳�
    moveToThread(&m_thread);
    m_thread.start();

	qRegisterMetaType<std::string>("std::string");
}

MainController::~MainController()
{
	m_thread.quit();
	if (false == m_thread.wait(3000))
	{
		loge() << "���������߳��˳��ȴ���ʱ��";
		m_thread.terminate();
	}
	disconnect(m_pSrvCnnt, SIGNAL(signalTcpConnectNotify(uint, bool)), this, SLOT(slotTcpConnectNotify(uint, bool)));
	disconnect(m_pSrvCnnt, SIGNAL(signalTcpDisconnectNotify(uint)), this, SLOT(slotTcpDisconnectNotify(uint)));
	m_pSrvCnnt = nullptr;
	m_pMainModel = nullptr;
}

bool MainController::Initialize()
{
	PbMessageHandle* pPbMsgHdl = m_pMsgHandle->GetPbMessageHandle();
	pPbMsgHdl->RegisterMessageFunction(pbmsg::ELoginRs, std::bind(&MainController::HandleLoginRs, this, std::placeholders::_1, std::placeholders::_2));

    m_pMainModel = m_pModel->GetMainModel();
     if(nullptr == m_pMainModel)
     {
         loge()<< "��ȡ��ģ��ʧ��!";
         return false;
     }

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

	PbMessageHandle* pPbMsgHdl = m_pMsgHandle->GetPbMessageHandle();
	pPbMsgHdl->RemoveMessageFunction(pbmsg::ELoginRs);
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1����½web
	emit signalLoginTipMsg("��ʼ��½web��֤�˻���������...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();
	//m_pWebCnnt->UserLogin(strUserName, strPassword, strMac);

	SUserInfo user;
	user.m_uUserId = 1;
	user.m_strUserName = strUserName;
	user.m_strPassword = strPassword;
	user.m_strMac = strMac;
	if (0 == user.m_uUserId)
	{
		emit signalLoginMessageNt(false, "��½webʧ�ܣ�");
		return;
	}
	m_pMainModel->SaveUserInfo(user);

	//2����web��ȡ������Ϣ
	signalLoginTipMsg("��ʼ��ȡ�ͻ���������Ϣ...");
	ClientConfig* pCfg = m_pMain->GetClientConfig();
	/*if (false == m_pWebCnnt->GetConfigInfo(user.m_strMac, pCfg))
	{
		loge() << "��ȡweb������������Ϣʧ�ܣ�";
		//emit signalLoginMessageNt(false, "��ȡweb������������Ϣʧ�ܣ�");
		//return; ʧ����û��ϵ�����ػ���������
		}*/
	emit signalLoginMessageNt(true, "���ӷ������ɹ���");
}

void MainController::slotExecuteSystem()
{
	//3�����TCP����
	emit signalSplashMessage("��ʼ���ӷ�����...");
	int iUserType = 1;
	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
	ClientConfig* cfg = m_pMain->GetClientConfig();

	for (int i = EST_CMD_SERVER; i < EST_SERVER_COUNT; i++)
	{
		if (false == m_pSrvCnnt->AddConnect(i, cfg->strServerIP, cfg->usServerPort, 1))
		{
			loge() << "��Ӷ����������������ʧ�ܣ�";
			emit signalSplashMessage("���ӷ�����ʧ�ܣ�");
			emit signalShowMainWindow(iUserType);		//�˴����ݾ����ҵ������
			return;
		}
	}
	emit signalShowMainWindow(iUserType);
}

void MainController::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("���ӷ���Key:%d�ɹ���", uServerType);

		SUserInfo& user = m_pMainModel->GetUserInfo();

		SLoginRq msg;
		msg.uUserId = uServerType;
		msg.strUserName = user.m_strUserName + std::to_string(uServerType);
		msg.strPassword = user.m_strPassword;
		m_pSrvCnnt->SendData(uServerType, msg.SerializeAsPbMsg());
	}
	else
	{
		loge() << "���ӷ���uServerType:" << uServerType << "ʧ�ܣ�";
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	m_pMainModel->DelLoginServer(uServerType);
	LOGM("����Key:%d�����ӶϿ�", uServerType);
}

void MainController::HandleHeartbeat()
{
	m_hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hHeartbeatEvent)
	{
		loge() << "���������¼�ʧ�ܣ�";
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
			logm() << "�����˳������̡߳�";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "�����̷߳�������" << ::GetLastError();
			break;
		}

		QSet<unsigned> setSrvKey = m_pMainModel->GetLoginServerList();
		for (unsigned uSrvKey : setSrvKey)
		{
			m_pSrvCnnt->SendData(uSrvKey, pbMsg);
		}
	}
}

void MainController::HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::auto_ptr<SRespondMsg> pRs(static_cast<SRespondMsg*>(pMsg));

	if (0 == pRs->uResult)
	{
		LOGM("��¼����Key:%d �ɹ���", uUserKey);
		//if (EST_CMD_SERVER == uUserKey)
		//{
		//	int iUserType = 1;
		//	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
		//	emit signalShowMainWindow(iUserType);
		//}
	}
	else
		LOGE("��¼����Key:%d ʧ�ܣ�", uUserKey);
}
