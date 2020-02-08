#pragma once
/************************************************************************
* filename:MainController.h
* function:主控制器，用于程序启动以及用户登陆业务的处理。
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include "ControllerColleague.h"
#include "../../CommonFile/TypeDefine.h"
#include <thread>

class UserInfoManage;

class MainController : public ControllerColleague
{
public:
    explicit MainController(IControllerManage*);
    ~MainController() override;

    bool Initialize();
    void Uninitialize();

	bool StartServer();
	void StopServer();

	void HandleLoginRq(const unsigned uUserKey, SDataExchange* pMsg);

	void HandleHeartbeatNt(const unsigned uUserKey, SDataExchange* pMsg);

private:
	void HeartbeatHandle();

private:
	UserInfoManage   *m_pUserMng = nullptr;
	TCPServer		 *m_pTcpSrv = nullptr;
	std::thread		 *m_pHeartbeatThread = nullptr;	//心跳线程
	HANDLE 			 m_hHeartbeatEvent = nullptr;	//心跳事件
};

