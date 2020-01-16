#pragma once
/************************************************************************
* filename:ControllerManage.h
* function: 主控制器，用于管理各个控制器
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include "IControllerManage.h"

class ControllerManage : public IControllerManage
{
public:
    ControllerManage(IMainClient *_main = nullptr);
    ~ControllerManage() override;
    MainController* GetMainController() override;

	bool Start() override;
	void Stop() override;

	//登陆成功之后调用这里
	void ExecuteSystem() override;

private:
    MainController      *m_pMainCtrl = nullptr;
};

