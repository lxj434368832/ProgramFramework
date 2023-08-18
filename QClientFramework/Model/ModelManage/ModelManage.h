#pragma once
/************************************************************************
* filename:ModelManage.h
* function: ģ�͹����������ڹ������ģ��
* author :	������
* datetime:	2018-12-14
* company:  
/************************************************************************/

#include "IModelManage.h"

class ICommunication;
class IMessageHandle;

class ModelManage : public IModelManage
{
public:
    ModelManage(IMainClient *_main = nullptr);
    ~ModelManage() override;
	MainModel* GetMainModel() override;

	bool Initialize() override;
	void Uninitialize() override;

private:
    MainModel           *m_pMainModel;
};

