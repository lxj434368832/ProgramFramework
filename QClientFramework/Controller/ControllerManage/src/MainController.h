#pragma once
/************************************************************************
* filename:MainController.h
* function:�������������ڳ��������Լ��û���½ҵ��Ĵ���
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QThread>
#include <thread>
#include <QSet>
#include "ControllerColleague.h"
#include "..\..\..\CommonFile\TypeDefine.h"
#include "..\..\..\3rdParty\Framework\include\MLock.h"
#include "..\include\IMainController.h"

struct SDataExchange;

class MainController : public IMainController, public ControllerColleague
{
    Q_OBJECT
public:
	explicit MainController(IControllerManage*);
    ~MainController();

    bool Initialize() override;
	void Uninitialize() override;

private slots:
	/********************����Ϊ�ⲿ�첽ִ�������ʵ�ֲ�********************/
	//�û���½
	void slotUserLogin(std::string strUserName, std::string strPassword);

	//ִ��ϵͳ
	void slotExecuteSystem();


private slots:
	/*************************����Ϊ�����Զ��ۺ���*************************/
	/*************************************************************************
	* function��		 Tcp����״̬֪ͨ
	* param uServerType: ����������
	* param state:		 ���ӳɹ���ʶ
	* param once:		 ��һ�����ӱ�ʶ
	*************************************************************************/
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
	/*************************����Ϊ��Ϣ�ص�����*************************/
	void HandleHeartbeat();

	//�����¼��Ӧ
	void HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg);

private:
    QThread			m_thread;                   //�첽�߳�
	std::thread		 *m_pHeartbeatThread = nullptr;	//�����߳�
	HANDLE 			 m_hHeartbeatEvent = nullptr;	//�����¼�

	SUserInfo		m_user;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};

