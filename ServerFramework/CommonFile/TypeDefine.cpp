#include "TypeDefine.h"
#include "CommonDefine.h"
#include "Message.pb.h"

void SUserInfo::Reset()
{
	uUserKey = 0;
	uUserId = 0;
	strName.clear();
	strPassword.clear();
	strSex.clear();
	unAge = 0;
	m_strFamilyAddress.clear();
	m_strCompanyAddress.clear();
}

bool SServerConfig::CheckValid()
{
	if (usListenPort == 0
		|| uInitAcceptCount == 0
		|| uServerThreadCount == 0
		|| uHeartbeatTime == 0)
	{
		return false;
	}
	return true;
}

bool SDataExchange::ParsePbMsg(const char* pData, unsigned uLength, ::google::protobuf::Message *pMsg)
{
	bool bSuc = false;
	try
	{
		if (pMsg->ParseFromArray(pData, uLength))
		{
			bSuc = true;
		}
		else
		{
			loge() << "解析" << strStructName << "失败，data:" << pData;
		}
	}
	catch (std::exception* e)
	{
		loge() << "解析" << strStructName << "异常:" << e->what() << "\ndata:" << pData;
	}
	return bSuc;
}

bool SDataExchange::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "protobuf 消息";
	//::google::protobuf::Message msg;
	//if (false == ParsePbMsg(pData, uLength, &msg)) return false;
	//to do msg assign

	return true;
}

bool SDataExchange::ParseFromPbMsg(::google::protobuf::Message *pMsg)
{
	strStructName = "protobuf 消息";
	//to do param assign

	return true;
}

SPbMsg SDataExchange::SerializeAsPbMsg()
{
	pbmsg::ClientMsg msg;
	msg.set_msg_type(pbmsg::ELoginRq);
	msg.set_msg_data("客户端消息");
	return PackPbMsg(pbmsg::ELoginRq, msg.SerializeAsString());
}

SPbMsg SDataExchange::PackPbMsg(unsigned uMsgType, std::string strMsg)
{
	SPbMsg pbMsg;
	pbMsg.uMsgType = uMsgType;
	pbMsg.strMsg = strMsg;
	return pbMsg;
}

SPbMsg SRespondMsg::SerializeAsPbMsg()
{
	pbmsg::RespondMsg msg;
	msg.set_result(0);
	msg.set_result_msg(strMsg);
	return PackPbMsg(uRsMsgType, msg.SerializeAsString());
}

bool SHeartBeatNt::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "心跳消息";
	pbmsg::LoginRq rq;
	if (false == ParsePbMsg(pData, uLength, &rq)) return false;
	uUserId = rq.user_id();
	return true;
}

bool SLoginRq::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "登录消息";
	pbmsg::LoginRq rq;
	if (false == ParsePbMsg(pData, uLength, &rq)) return false;
	uUserId = rq.user_id();
	strUserName = rq.username();
	strPassword = rq.password();
	return true;
}
