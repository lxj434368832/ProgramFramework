#pragma once
/*************************************************************************
* function：服务器连接管理的定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "MLock.h"
#include <atomic>
#include <map>

struct UserInfo;

class ServerManage
{
public:
	virtual ~ServerManage();

	unsigned GetSrvUserId();

	//新服务用户成功连接通知
	virtual void SrvAcceptNotify(unsigned uUserId, SOCKET s);

	//处理服务数据
	virtual void HandSrvData(unsigned uUserId, const char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	UserInfo* DeleteSrvUser(unsigned userId);

protected:
	virtual UserInfo* CreateSrvUserInfo() = 0;

	virtual void ReleaseSrvUserInfo(UserInfo* pSrvUserInfo) = 0;

protected:
	std::map<unsigned, UserInfo*> m_mapSrvUserList;	//服务用户列表
	MLock						 m_lckSrvUserList;	//服务用户列表锁

private:
	std::atomic<unsigned> m_uSrvUidGenerator = 1;
};
