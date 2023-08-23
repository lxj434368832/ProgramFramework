#pragma once

#include <QObject>

class IControllerManage;

class IMainController : public QObject
{
	Q_OBJECT
public:
	static IMainController* Create(IControllerManage*);
	static void Delete(IMainController*&);
	explicit IMainController() = default;
	virtual ~IMainController() = default;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

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

};
