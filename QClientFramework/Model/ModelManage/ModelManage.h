#pragma once
/************************************************************************
* filename:ModelManage.h
* function: 模型管理器，用于管理各个模型
* author :	明巧文
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
	ICommunication* GetCommunication() override;
	IMessageHandle*	GetMessageHandle() override;

	bool Initialize() override;
	void Uninitialize() override;

private:
	ICommunication		*m_pCmmnt;
	IMessageHandle      *m_pMsgHandle;
    MainModel           *m_pMainModel;
};

