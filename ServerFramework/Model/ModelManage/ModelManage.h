#pragma once

#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
	ModelManage(IMainServer* srv = nullptr);
	~ModelManage();
	UserInfoManage* GetUserInfoManage() override;

	bool Start() override;
	void Stop() override;

private:
	UserInfoManage*	m_pUserInfo;
};

