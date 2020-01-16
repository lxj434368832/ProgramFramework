#include "HandleRespondMessage.h"
#include "IMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"



HandleRespondMessage::HandleRespondMessage(IMessageHandle *pMsgHandle)
	:IMessageColleague(pMsgHandle)
{
	ProtobufMsgFactory *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	//pMsgFctry->RegisterMessageFunction(pbmsg::MSG::ELoginRequest, std::bind(&HandleRequestMessage::HandleLoginRequest,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleRespondMessage::~HandleRespondMessage()
{
}

std::string HandleRespondMessage::BuildLoginRespond(bool bResult, std::string strMsg)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::ELoginRequest);
	msg.set_sequence(1);
	pbmsg::Respond *msgRs = msg.mutable_respond();
	msgRs->set_result(bResult);
	msgRs->set_lastrespond(true);
	pbmsg::LoginRespond *msgLogin = msgRs->mutable_login();
	msgLogin->set_ttl(1);
	return msg.SerializeAsString();
}
