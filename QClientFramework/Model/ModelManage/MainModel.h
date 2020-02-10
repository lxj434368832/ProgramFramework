#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include <vector>


class MainModel
{
public:
    MainModel();

    bool Initialize();
	void Uninitialize();

	//新服务用户成功连接通知
	void AddServerUser(unsigned uUserKey);

	// 删除服务用户
	void DeleteServerUser(unsigned uUserKey);

	SUserInfo* GetServerUser(unsigned uUserKey);

	/*************************************************************************
	* function：锁住UserKey对应的信息
	*************************************************************************/
	void LockServerUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function：解锁UserKey对应的信息
	*************************************************************************/
	void UnlockServerUserInfo(UserKey uUserKey);

	std::vector<unsigned> GetLoginUserList();

	SUserInfo& GetUserInfo();

	void SaveUserInfo(SUserInfo& user);

private:
	mqw::ResourceManage<SUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserKey, SUserInfo*>	m_mapUserList;		//用户key和UserInfo的映射
	MLock							m_lckUserList;		//用户列表锁

	SUserInfo						m_user;
};

#endif // MAINMODEL_H
