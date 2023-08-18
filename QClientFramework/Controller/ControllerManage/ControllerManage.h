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
	bool Initialize() override;
	void Uninitialize() override;

	MainController* GetMainController() override;
	ICommunication* GetCommunication() override;
	IMessageHandle* GetMessageHandle() override;

private:
	ICommunication* m_pCmmnt = nullptr;
	IMessageHandle* m_pMsgHandle = nullptr;
    MainController      *m_pMainCtrl = nullptr;
};

