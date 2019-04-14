#pragma once

#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
	ModelManage(IMainServer* pMain = nullptr);
	~ModelManage();
	UserInfoManage* GetUserInfoManage() override;

	bool Start() override;
	void Stop() override;

private:
	UserInfoManage*	m_pUserInfo;
};

