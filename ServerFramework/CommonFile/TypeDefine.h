#pragma once

#include <string>

typedef unsigned UserKey;
typedef unsigned UserId;

struct ClientUserInfo
{
	unsigned	m_uUserKey;
	unsigned	m_uUserId;
	std::string	m_strName;
	std::string m_strPassword;
	std::string m_strSex;
	unsigned	m_unAge;
	std::string m_strFamilyAddress;
	std::string m_strCompanyAddress;

	void Reset();
};

struct ServerConfig
{
	unsigned short  usListenPort = 6666;         // 监听端口
	unsigned int    uServerThreadCount = 4;			// 完成端口监听工作线程数量
	unsigned int    uMessageThreadCount = 4;			// 消息处理线程数量
	unsigned int    uInitAcceptCount = 10;           // 服务端初始接受个数
	unsigned int    uHeartbeatTime = 6000;			// 定时心跳检测时间ms

	// 需要判断读取出来的配置是否正确
	bool CheckValid();
};
