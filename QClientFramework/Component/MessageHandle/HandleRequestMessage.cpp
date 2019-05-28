#include"HandleRequestMessage.h"
#include "IMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"

HandleRequestMessage::HandleRequestMessage(IMessageHandle *pMsgHandle)
	:IMessageColleague(pMsgHandle)
{

}

HandleRequestMessage::~HandleRequestMessage()
{

}

std::string HandleRequestMessage::BuildLoginRequest(std::string strUserName, std::string strPassword)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::ELoginRequest);
	msg.set_sequence(1);

	pbmsg::Request *msgRq = msg.mutable_request();
	pbmsg::LoginRequest *loginRq = msgRq->mutable_login();
	loginRq->set_username(strUserName);
	loginRq->set_password(strPassword);
	std::string strMsg = msg.SerializeAsString();

	return msg.SerializeAsString();
}
