#pragma once
/************************************************************************
* filename:MainController.h
* function:主控制器，用于程序启动以及用户登陆业务的处理。
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QThread>
#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(ControllerManage*);
    ~MainController() override;

    bool Start();
	void Stop();

	void UserLoginResult(unsigned uUserKey, bool bRet, std::string strMsg);

signals:

private slots :
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
    MainModel   *m_pMainModel;
    QThread       m_thread;                   //异步线程
};

