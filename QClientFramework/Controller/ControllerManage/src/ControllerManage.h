#pragma once
/************************************************************************
* filename:ControllerManage.h
* function: �������������ڹ������������
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include "../include/IControllerManage.h"

class ControllerManage : public IControllerManage
{
public:
    ControllerManage(IMainClient *_main = nullptr);
	~ControllerManage() override;
	bool Initialize() override;
	void Uninitialize() override;

	IMainController* GetMainController() override;

private:
	IMainController      *m_pMainCtrl = nullptr;
};

