#include"HandleRequestMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"

HandleRequestMessage::HandleRequestMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{

}

HandleRequestMessage::~HandleRequestMessage()
{

}

std::string HandleRequestMessage::BuildLoginRequest(std::string strUserName, std::string strPassword)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::ELoginRq);
	msg.set_sequence(1);

	pbmsg::Request *msgRq = msg.mutable_request();
	pbmsg::LoginRq *loginRq = msgRq->mutable_login();
	loginRq->set_username(strUserName);
	loginRq->set_password(strPassword);
	std::string strMsg = msg.SerializeAsString();

	return msg.SerializeAsString();
}
