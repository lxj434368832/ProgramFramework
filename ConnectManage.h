#ifndef CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A
#define CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A
/*************************************************************************
* function：连接管理定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "ResourceManage.h"
#include <atomic>
#include <map>
#include "MLock.h"

struct UserInfo;

class ConnectManage
{
public:
	ConnectManage();
	virtual ~ConnectManage();

	unsigned GetCnntUserId();

	virtual void ConnectNotify(unsigned uUserId, SOCKET socket, bool bSuccess);

	// 返回删除用户的地址，返回NULL代表用户不存在
	virtual UserInfo* DeleteCnntUser(unsigned UserId);

	// 此函数为虚函数，需要子类实现具体的业务
	virtual void HandCnntData(unsigned uUserId, const char* data, unsigned length);

	SOCKADDR_IN* GetSOCKADDR();

	void RecycleSOCKADDR(SOCKADDR_IN* addr);


protected:
	virtual UserInfo* CreateCnntUserInfo() = 0;

	virtual void ReleaseCnntUserInfo(UserInfo* pUserInfo) = 0;

protected:
	std::map<unsigned, UserInfo*> m_mapCnntUserList;		//已经连接的用户管理
	MLock						  m_lckUserList;			//用户列表锁

private:
	std::atomic<unsigned> m_uCnntUserIdGenerator = 1;

	mqw::ResourceManage<SOCKADDR_IN> m_rscAddr;
};


#endif /* CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A */
