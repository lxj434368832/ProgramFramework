#include "MainModel.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"

MainModel::MainModel(IModelManage* pMdlMng) :
	ModelColleague(pMdlMng)
{
}

MainModel::~MainModel()
{
}

bool MainModel::Initialize()
{
    return true;
}

void MainModel::Uninitialize()
{
}

void MainModel::SaveUserInfo(SUserInfo& user)
{
	m_user = user;
}

void MainModel::LockUserInfo()
{
	m_lckUserInfo.lock();
}

void MainModel::UnlockUserInfo()
{
	m_lckUserInfo.unlock();
}

void MainModel::AddLoginServer(UserKey uUserKey)
{
}

void MainModel::DelLoginServer(UserKey uUserKey)
{
}

QSet<unsigned> MainModel::GetLoginServerList()
{
	return m_setSrvUser;
}

SUserInfo& MainModel::GetUserInfo()
{
	return m_user;
}
