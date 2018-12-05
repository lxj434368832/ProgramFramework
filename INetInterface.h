#pragma once

#include <map>
#include "UserInfo.h"

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

	//新服务用户成功连接通知
	virtual void AddUser(unsigned uUserKey);

	//处理服务数据
	virtual void HandSrvData(unsigned uUserKey, const char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(unsigned uUserKey);

protected:
	std::map<unsigned, UserInfo> m_mapUserList;

};

