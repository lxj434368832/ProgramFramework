#pragma once

#include "EnumDefine.h"
#include <string>

typedef unsigned UserKey;

struct ClientConfig
{
	std::string		strServerIP = "";				//���ӵ�IP
	unsigned short  usServerPort = 6666;			//���Ӷ˿�
	unsigned int    uServerThreadCount = 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;		// ��Ϣ�����߳�����
	unsigned int    uHeartbeatTime = 6000;			// ��ʱ��������ʱ��ms

	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
    bool CheckValid();
};

struct SUserInfo
{
	unsigned		m_uUserKey = 0;
	unsigned		m_uUserId = 0;
	std::string		m_strUserName;
	std::string		m_strPassword;
	std::string		m_strUserDesc;
	std::string		m_strMac;
	bool			m_bLogin = false;
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

//���ݽ����Ĺ����ṹ
struct SDataExchange
{
	std::string strStructName;//�ṹ������

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

	bool ParseFromPb(const char* pData, unsigned uLength) override;
};

struct SHeartBeatNt : SDataExchange
{
	unsigned	uUserId = 0;

	virtual SPbMsg SerializeAsPbMsg();
};

struct SLoginRq : SDataExchange
{
	unsigned		uUserId = 0;
	std::string		strUserName;
	std::string		strPassword;

	virtual SPbMsg SerializeAsPbMsg();
};
