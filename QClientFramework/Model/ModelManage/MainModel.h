#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"

class MainModel
{
public:
    MainModel();

    bool Start();
	void Stop();

	//新服务用户成功连接通知
	void AddUser(unsigned uUserKey);

	// 删除用户
	void DeleteUser(unsigned uUserKey);

private:
	mqw::ResourceManage<SUserInfo>	m_rscUser;
	//MLock				m_shareLock[CONNECT_SHARE_LOCK_COUNT];

	std::map<UserKey, SUserInfo*>	m_mapUserList;		//用户key和UserInfo的映射
	MLock							m_lckUserList;		//用户列表锁
};

#endif // MAINMODEL_H
