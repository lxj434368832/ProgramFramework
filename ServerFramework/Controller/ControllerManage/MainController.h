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

class UserInfoManage;

class MainController : public ControllerColleague
{
public:
    explicit MainController(IControllerManage*);
    ~MainController() override;

    bool Start();
    void Stop();

	void UserLogin(ClientUserInfo &userInfo);
private:
	UserInfoManage   *m_pUserMng;
};

