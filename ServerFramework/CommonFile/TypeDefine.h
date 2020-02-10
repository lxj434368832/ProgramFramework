#pragma once

#include <string>

typedef unsigned UserKey;
typedef unsigned UserId;

struct SServerConfig
{
	unsigned short  usListenPort = 6666;         // 监听端口
	unsigned int    uServerThreadCount = 4;			// 完成端口监听工作线程数量
	unsigned int    uMessageThreadCount = 4;			// 消息处理线程数量
	unsigned int    uInitAcceptCount = 10;           // 服务端初始接受个数
	unsigned int    uHeartbeatTime = 6000;			// 定时心跳检测时间ms

	// 需要判断读取出来的配置是否正确
	bool CheckValid();
};

struct SUserInfo
{
	unsigned	uUserKey;
	unsigned	uUserId;
	std::string	strLoginName;
	std::string strPassword;
	std::string	strName;
	std::string strSex;
	unsigned	uAge;
	unsigned	uHeartCount = 0;
	void Reset();
};

//namespace Json {
//	class Value;
//}
namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

struct SPbMsg
{
	unsigned	uMsgType;
	std::string	strMsg;
};

//数据交换的公共结构
struct SDataExchange
{
	std::string strStructName;//结构中文名

	bool ParsePbMsg(const char* pData, unsigned uLength, ::google::protobuf::Message *pMsg);
	virtual bool ParseFromPb(const char* pData, unsigned uLength);
	virtual bool ParseFromPbMsg(::google::protobuf::Message *pMsg);

	virtual SPbMsg SerializeAsPbMsg();
	SPbMsg PackPbMsg(unsigned uMsgType, std::string strMsg);
};

struct SRespondMsg : SDataExchange
{
	unsigned	uRsMsgType = 0;
	unsigned	uResult = 0;
	std::string strMsg;

	virtual SPbMsg SerializeAsPbMsg();
};

struct SHeartBeatNt : SDataExchange
{
	unsigned	uUserId = 0;

	bool ParseFromPb(const char* pData, unsigned uLength) override;
};

struct SLoginRq : SDataExchange
{
	unsigned		uUserId = 0;
	std::string		strUserName;
	std::string		strPassword;

	bool ParseFromPb(const char* pData, unsigned uLength) override;
};
