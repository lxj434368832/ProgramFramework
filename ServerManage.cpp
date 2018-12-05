#include "stdafx.h"
#include "UserInfo.h"
#include "ServerManage.h"

ServerManage::~ServerManage()
{

}

unsigned ServerManage::GetSrvUserId()
{
	return m_uSrvUidGenerator ++;
}

void ServerManage::SrvAcceptNotify(unsigned uUserId, SOCKET s)
{
	UserInfo *pSrv = CreateSrvUserInfo();
	if (pSrv)
	{
		pSrv->m_uUserId = uUserId;
		pSrv->m_socket = s;

		MAutoLock lck(&m_lckSrvUserList);
		m_mapSrvUserList[uUserId] = pSrv;
	}
}

void ServerManage::HandSrvData(unsigned uUserId, const char* data, unsigned length)
{
}

UserInfo* ServerManage::DeleteSrvUser(unsigned userId)
{
	UserInfo *pSrv = NULL;
	{
		MAutoLock lck(&m_lckSrvUserList);
		auto it = m_mapSrvUserList.find(userId);
		if (m_mapSrvUserList.end() != it)
		{
			pSrv = it->second;
			m_mapSrvUserList.erase(it);
		}
	}

	if (pSrv)
	{
		ReleaseSrvUserInfo(pSrv);
	}

	return pSrv;
}

