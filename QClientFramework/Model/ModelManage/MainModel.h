#pragma once

#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include <QSet>

class MainModel
{
public:
    MainModel();

    bool Initialize();
	void Uninitialize();

	void SaveUserInfo(SUserInfo& user);

	SUserInfo& GetUserInfo();

	/*************************************************************************
	* function：锁住UserKey对应的信息
	*************************************************************************/
	void LockUserInfo();

	/*************************************************************************
	* function：解锁UserKey对应的信息
	*************************************************************************/
	void UnlockUserInfo();

	void AddLoginServer(UserKey uUserKey);

	void DelLoginServer(UserKey uUserKey);

	QSet<unsigned> GetLoginServerList();

private:
	SUserInfo	m_user;
	MLock		m_lckUserInfo;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};
