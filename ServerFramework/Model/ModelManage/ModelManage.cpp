#include "ModelManage.h"
#include "..\..\MainServer\MainServer.h"
#include "UserInfoManage.h"
#include "..\..\CommonFile\CommonDefine.h"

ModelManage::ModelManage(IMainServer* srv)
	:IModelManage(srv)
{
	m_pUserInfo = new UserInfoManage(this);
}

ModelManage::~ModelManage()
{
	RELEASE(m_pUserInfo);
}

UserInfoManage* ModelManage::GetUserInfoManage()
{
	return m_pUserInfo;
}

bool ModelManage::Start()
{
	return m_pUserInfo->Start();
}

void ModelManage::Stop()
{
	m_pUserInfo->Stop();
}
