#include "TypeDefine.h"
#include "CommonDefine.h"
#include <Message.pb.h>

bool ClientConfig::CheckValid()
{
        if (strServerIP.empty()
            || 0== usServerPort
            || 0 == uServerThreadCount
            || 0 == uHeartbeatTime )
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
			loge() << "����" << strStructName << "ʧ�ܣ�data:" << pData;
		}
	}
	catch (std::exception* e)
	{
		loge() << "����" << strStructName << "�쳣:" << e->what() << "\ndata:" << pData;
	}
	return bSuc;
}

bool SDataExchange::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "protobuf ��Ϣ";
	//::google::protobuf::Message msg;
	//if (false == ParsePbMsg(pData, uLength, &msg)) return false;
	//to do msg assign

	return true;
}

bool SDataExchange::ParseFromPbMsg(::google::protobuf::Message *pMsg)
{
	strStructName = "protobuf ��Ϣ";
	//to do param assign

	return true;
}

SPbMsg SDataExchange::SerializeAsPbMsg()
{
	pbmsg::ClientMsg msg;
	msg.set_msg_type(pbmsg::ELoginRq);
	msg.set_msg_data("�ͻ�����Ϣ");
	return PackPbMsg(pbmsg::ELoginRq, msg.SerializeAsString());
}

SPbMsg SDataExchange::PackPbMsg(unsigned uMsgType, std::string strMsg)
{
	SPbMsg pbMsg;
	pbMsg.uMsgType = uMsgType;
	pbMsg.strMsg = strMsg;
	return pbMsg;
}

bool SRespondMsg::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "��Ӧ��Ϣ";
	pbmsg::RespondMsg msg;
	if (false == ParsePbMsg(pData, uLength, &msg)) return false;
	uResult = msg.result();
	strMsg = msg.result_msg();
	return true;
}

SPbMsg SHeartBeatNt::SerializeAsPbMsg()
{
	pbmsg::HeartbeatNt msg;
	msg.set_user_id(uUserId);
	return PackPbMsg(pbmsg::EHeartbeatNt, msg.SerializeAsString());
}

SPbMsg SLoginRq::SerializeAsPbMsg()
{
	pbmsg::LoginRq msg;
	msg.set_user_id(uUserId);
	msg.set_username(strUserName);
	msg.set_password(strPassword);
	return PackPbMsg(pbmsg::ELoginRq, msg.SerializeAsString());
}
