#include "stdafx.h"
#include "ConnectManage.h"
#include "UserInfo.h"

ConnectManage::ConnectManage() :m_rscAddr(5)
{

}

ConnectManage::~ConnectManage()
{

}

unsigned ConnectManage::GetCnntUserId()
{
	return m_uCnntUserIdGenerator++;
}

void ConnectManage::ConnectNotify(unsigned uUserId, SOCKET socket, bool bSuccess)
{
	if (bSuccess)
	{
		UserInfo *pInfo = CreateCnntUserInfo();
		if (NULL != pInfo)
		{
			pInfo->m_uUserId = uUserId;
			pInfo->m_socket = socket;
		}
		MAutoLock autolck(&m_lckUserList);
		m_mapCnntUserList[uUserId] = pInfo;
	}
}

void ConnectManage::HandCnntData(unsigned uUserId, const char* data, unsigned length)
{
}

SOCKADDR_IN* ConnectManage::GetSOCKADDR()
{
	return m_rscAddr.get();
}

void ConnectManage::RecycleSOCKADDR(SOCKADDR_IN* addr)
{
	if (addr)
	{
		m_rscAddr.put(addr);
	}
}

UserInfo* ConnectManage::DeleteCnntUser(unsigned UserId)
{
	UserInfo *pUser = NULL;
	{
		MAutoLock autoLock(&m_lckUserList);
		auto it = m_mapCnntUserList.find(UserId);
		if (m_mapCnntUserList.end() != it)
		{
			pUser = it->second;
			m_mapCnntUserList.erase(it);
		}
	}
	if (pUser)
	{
		ReleaseCnntUserInfo(pUser);
	}

	return pUser;
}

