#pragma once
/*************************************************************************
* function：服务器连接管理的定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "../../3rdParty/Framework/include/MLock.h"
#include "../../CommonFile/TypeDefine.h"
#include "../../CommonFile/EnumDefine.h"
class IModelManage;

class UserInfoManage
{
public:
	UserInfoManage(IModelManage* pMgr);
	virtual ~UserInfoManage();

	bool Start();
	void Stop();

	//新服务用户成功连接通知
	void AddUser(UserKey uUserKey);

	//通讯层回调删除用户
	void DeleteUser(UserKey uUserKey);

	/*************************************************************************
	* function：获取客户端用户信息通过UserKey
	* return:	成功返回ClientUserInfo，失败返回nullptr
	*************************************************************************/
	ClientUserInfo* GetClientUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function：获取客户端用户信息通过UserId
	* return:	成功返回ClientUserInfo，失败返回nullptr
	*************************************************************************/
	ClientUserInfo* GetClientUserInfoById(UserId uUserId);

	/*************************************************************************
	* function：添加UserId和UserKey的映射关系，用于用户登录的时候调用
	* return:	空
	*************************************************************************/
	void AddUserIdUserKeyMap(UserId uUserId, UserKey uUserKey);

	/*************************************************************************
	* function：根据UserId和获取对应的UserKey
	* return:	UserKey
	*************************************************************************/
	UserKey GetUserKeyById(UserId uUserId);

	/*************************************************************************
	* function：根据UserKey获取当前用户对应的锁
	* return:	UserKey
	*************************************************************************/
	MLock*	GetClientUserLock(UserKey uUserKey);

	void SetUserInfo(UserKey uUserKey, ClientUserInfo &info);

private:
	IModelManage*	m_pMgr;

	mqw::ResourceManage<ClientUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserId, UserKey>		m_mapIdKey;			//用户id和key的映射关系
	MLock							m_lckIdKey;			//用户id和key的映射关系锁
	std::map<UserKey, ClientUserInfo*>	m_mapUserList;	//用户key和UserInfo的映射
	MLock							m_lckUserList;		//用户列表锁

};
