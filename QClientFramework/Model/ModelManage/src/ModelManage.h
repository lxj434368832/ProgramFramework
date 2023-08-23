#pragma once
/************************************************************************
* filename:ModelManage.h
* function: 模型管理器，用于管理各个模型
* author :	明巧文
* datetime:	2018-12-14
* company:  
/************************************************************************/

#include "..\include\IModelManage.h"

class ModelManage : public IModelManage
{
public:
    ModelManage(IMainClient *_main = nullptr);
    ~ModelManage() override;
	virtual IServerConnect* GetServerConnect() override;
	virtual IMessageHandle* GetPbMessageHandle() override;

	bool Initialize() override;
	void Uninitialize() override;

private:
	IServerConnect		*m_pConnect;
	IMessageHandle		*m_pPbMsgHandle;
};
