#include "MainModel.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"

MainModel::MainModel():
	m_rscUser(EST_SERVER_COUNT)
{

}

bool MainModel::Initialize()
{
    return true;
}

void MainModel::Uninitialize()
{

}

void MainModel::AddServerUser(unsigned uUserKey)
{
	MAutoLock lck(m_lckUserList);
	auto iter = m_mapUserList.find(uUserKey);
	if (iter != m_mapUserList.end() && iter->second)
	{
		LOGE("已经存在当前用户key:%d id:%d 无需添加!",uUserKey,iter->second->m_uUserId);
		return;
	}

	SUserInfo *pUser = m_rscUser.get();
	pUser->m_uUserKey = uUserKey;
	pUser->m_uUserId = 0;
	m_mapUserList[uUserKey] = pUser;
}

void MainModel::DeleteServerUser(unsigned uUserKey)
{
	SUserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() != iter)
	{
		pUser = iter->second;
	}
	m_mapUserList.erase(uUserKey);
	m_lckUserList.unlock();

	if (pUser)
	{
		m_rscUser.put(pUser);
	}
}

void MainModel::LockServerUserInfo(UserKey uUserKey)
{
	m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT].lock();
}

void MainModel::UnlockServerUserInfo(UserKey uUserKey)
{
	m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT].unlock();
}

std::vector<unsigned> MainModel::GetLoginUserList()
{
	std::vector<unsigned> userList;

	m_lckUserList.lock();
	for (auto iter = m_mapUserList.begin(); iter != m_mapUserList.end(); iter++)
	{
		SUserInfo *pUser = iter->second;
		if (pUser->m_bLogin)
		{
			userList.push_back(pUser->m_uUserKey);
		}
	}
	m_lckUserList.unlock();

	logm() << "当前登录用户数为：" << userList.size() << "用户总数为:"<< m_mapUserList.size();
	return userList;
}

SUserInfo& MainModel::GetUserInfo()
{
	return m_user;
}

void MainModel::SaveUserInfo(SUserInfo& user)
{
	m_user = user;
}
