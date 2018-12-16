#pragma once
/*************************************************************************
* function：服务器连接管理的定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "../../IOCPCommunication/INetInterface.h"
#include "../../Framework/ResourceManage.h"
#include "../TypeDefine.h"
#include "../EnumDefine.h"

class IManageBusiness;
class ProtobufMsgFactory;

class UserInfoManage : public INetInterface
{
public:
	UserInfoManage(IManageBusiness* pMgr);
	virtual ~UserInfoManage();

	bool Start();
	void Stop();

	//新服务用户成功连接通知
	void AddUser(UserKey uUserKey) override;
	//处理服务数据
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;
	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	void DeleteUser(UserKey uUserKey) override;

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

private:
	IManageBusiness*	m_pMgr;
	ProtobufMsgFactory* m_pMsgFctry;

	mqw::ResourceManage<ClientUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];
};
