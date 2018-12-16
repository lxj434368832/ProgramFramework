#pragma once

#include <map>
#include <functional>
#include "..\Framework\MLock.h"

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

typedef unsigned UserKey;
typedef unsigned UserId;

struct UserInfo
{
	UserKey	m_uUserKey = 0;
	UserId m_uUserId = 0;
};

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

	//注册回调函数
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//新服务用户成功连接通知
	virtual void AddUser(UserKey uUserKey);

	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(UserKey uUserKey);

protected:
	friend class IOCPBase;

	SendDataCallback				m_funSendData;		//发送数据回调
	DisconnectCallback				m_fuDisconnect;		//主动断开连接回调
	std::map<UserId, UserKey>		m_mapIdKey;			//用户id和key的映射关系
	MLock							m_lckIdKey;			//用户id和key的映射关系锁
	std::map<UserKey, UserInfo*>	m_mapUserList;		//用户key和UserInfo的映射
	MLock							m_lckUserList;		//用户列表锁
};

