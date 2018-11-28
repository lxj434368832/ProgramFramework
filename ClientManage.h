#pragma once
/*************************************************************************
* function：客户端器连接管理的定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "MLock.h"
#include <atomic>
#include <map>

struct UserInfo;

class ClientManage
{
public:
	ClientManage();
	virtual ~ClientManage();

	void SetClientListenSocket(SOCKET listenSocket);

	SOCKET GetClientListenSocket();

	unsigned GetClientUserId();

	//新服务用户成功连接通知
	virtual void ClientAcceptNotify(unsigned uUserId, SOCKET s);

	//处理服务数据
	virtual void HandClientData(unsigned uUserId, char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	UserInfo* DeleteClientUser(unsigned userId);

protected:
	virtual UserInfo* CreateClientUserInfo() = 0;

	virtual void ReleaseClientUserInfo(UserInfo* pSrvUserInfo) = 0;

protected:
	SOCKET m_sClientLstnSocket;

	std::map<unsigned, UserInfo*> m_mapClientUserList;	//客户端用户列表
	MLock						 m_lckClientUserList;	//客户端用户列表锁

private:
	std::atomic<unsigned> m_uClientUidGenerator = 1;

};

