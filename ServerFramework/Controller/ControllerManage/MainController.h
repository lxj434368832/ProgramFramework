#pragma once
/************************************************************************
* filename:MainController.h
* function:主控制器，用于程序启动以及用户登陆业务的处理。
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
public:
    explicit MainController(IMainServer*);
    ~MainController() override;

    bool Start();
    void Stop();

private:
    MainModel   *m_mainModel;
};

