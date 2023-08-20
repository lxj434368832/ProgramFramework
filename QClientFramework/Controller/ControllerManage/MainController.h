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
#include "ControllerColleague.h"
#include "..\..\CommonFile\TypeDefine.h"

class MainModel;
struct SDataExchange;
class ServerConnect;
namespace std{ class thread; }

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(ControllerManage*);
    ~MainController() override;

    bool Initialize();
	void Uninitialize();

signals:
	/*************************����Ϊ�����֪ͨ��Ϣ*************************/
	//��½��ʾ��Ϣ
	void signalLoginTipMsg(std::string strMsg);
	//��½��Ϣ֪ͨ
	void signalLoginMessageNt(bool bSuccess, std::string strMsg);
	//����������Ϣ
	void signalSplashMessage(std::string strMsg);
	//֪ͨ��ʾ������
	void signalShowMainWindow(unsigned uUserType);

	//������ʾ��Ϣ
	void signalPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

signals:
	/************************����Ϊ�ⲿ�첽ִ������************************/
	//�û���½
	void signalUserLogin(std::string strUserName, std::string strPassword);
	//ִ��ϵͳ
	void signalExecuteSystem();

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
	ServerConnect* m_pSrvCnnt = nullptr;		//���������
    QThread			m_thread;                   //�첽�߳�
	std::thread		 *m_pHeartbeatThread = nullptr;	//�����߳�
	HANDLE 			 m_hHeartbeatEvent = nullptr;	//�����¼�

	SUserInfo		m_user;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};

