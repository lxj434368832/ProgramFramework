#include "MainModel.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"

MainModel::MainModel():
	m_rscUser(EST_SERVER_COUNT)
{

}

bool MainModel::Start()
{
    return true;
}

void MainModel::Stop()
{

}

void MainModel::AddUser(unsigned uUserKey)
{
	MAutoLock lck(&m_lckUserList);
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

void MainModel::DeleteUser(unsigned uUserKey)
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
