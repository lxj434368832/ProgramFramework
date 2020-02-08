#include "ModelManage.h"
#include "..\..\MainServer\MainServer.h"
#include "UserInfoManage.h"
#include "..\..\CommonFile\CommonDefine.h"

ModelManage::ModelManage(IMainServer* pMain)
	:IModelManage(pMain)
{
	m_pUserInfo = new UserInfoManage(this);
}

ModelManage::~ModelManage()
{
	RELEASE(m_pUserInfo);
}

MainModel* ModelManage::GetMainModel()
{
	return m_pMainModel;
}

UserInfoManage* ModelManage::GetUserInfoManage()
{
	return m_pUserInfo;
}

bool ModelManage::Initialize()
{
	return m_pUserInfo->Initialize();
}

void ModelManage::Uninitialize()
{
	m_pUserInfo->Uninitialize();
}
