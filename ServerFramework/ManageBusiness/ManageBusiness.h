#pragma once

#include "IManageBusiness.h"

class ManageBusiness : public IManageBusiness
{
public:
	ManageBusiness(IMainServer* srv = nullptr);
	~ManageBusiness();
	UserInfoManage* GetUserInfoManage() override;

	bool Start() override;
	void Stop() override;

private:
	UserInfoManage*	m_pUserInfo;
};

