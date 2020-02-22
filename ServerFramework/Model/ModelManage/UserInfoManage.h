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
#include <vector>

class IModelManage;

class UserInfoManage
{
public:
	UserInfoManage(IModelManage* pMgr);
	virtual ~UserInfoManage();

	bool Initialize();
	void Uninitialize();

	//新服务用户成功连接通知
	void AddUser(UserKey uUserKey);

	//通讯层回调删除用户
	void DeleteUser(UserKey uUserKey);

	/*************************************************************************
	* function：获取客户端用户信息通过UserKey
	* return:	成功返回ClientUserInfo，失败返回nullptr
	*************************************************************************/
	SUserInfo* GetClientUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function：获取客户端用户信息通过UserId
	* return:	成功返回ClientUserInfo，失败返回nullptr
	*************************************************************************/
	SUserInfo* GetClientUserInfoById(UserId uUserId);

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
	* function：锁住UserKey对应的信息
	*************************************************************************/
	void LockUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function：解锁UserKey对应的信息
	*************************************************************************/
	void UnlockUserInfo(UserKey uUserKey);

	void ClearUserHeartbeatCount(UserKey uUserKey);

	std::vector<unsigned> GetOfflineUserList();

private:
	IModelManage*	m_pMgr;

	mqw::ResourceManage<SUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserId, UserKey>		m_mapIdKey;			//用户id和key的映射关系
	MLock							m_lckIdKey;			//用户id和key的映射关系锁
	std::map<UserKey, SUserInfo*>	m_mapUserList;	//用户key和UserInfo的映射
	MLock							m_lckUserList;		//用户列表锁

};
