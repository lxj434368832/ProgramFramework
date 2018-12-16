#include "ManageBusiness.h"
#include "..\MainServer.h"
#include "UserInfoManage.h"
#include "..\CommonDefine.h"

ManageBusiness::ManageBusiness(IMainServer* srv)
	:IManageBusiness(srv)
{
	m_pUserInfo = new UserInfoManage(this);
}

ManageBusiness::~ManageBusiness()
{
	RELEASE(m_pUserInfo);
}

UserInfoManage* ManageBusiness::GetUserInfoManage()
{
	return m_pUserInfo;
}

bool ManageBusiness::Start()
{
	return m_pUserInfo->Start();
}

void ManageBusiness::Stop()
{
	m_pUserInfo->Stop();
}
