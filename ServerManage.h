#pragma once
/*************************************************************************
* function：服务器连接管理的定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "INetInterface.h"

struct UserInfo;

class ServerManage : public INetInterface
{
public:
	ServerManage();
	virtual ~ServerManage();

	//新服务用户成功连接通知
	virtual void AddUser(unsigned uUserKey);

	//处理服务数据
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(unsigned uUserKey);

private:

};
