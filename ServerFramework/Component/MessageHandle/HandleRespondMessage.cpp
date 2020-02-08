#include "HandleRespondMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include "Message.pb.h"


HandleRespondMessage::HandleRespondMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{
	//PbMessageHandle *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	//pMsgFctry->RegisterMessageFunction(pbmsg::MSG::ELoginRequest, std::bind(&HandleRequestMessage::HandleLoginRequest,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleRespondMessage::~HandleRespondMessage()
{
}

pbmsg::Respond* BuildRespondMessage(pbmsg::Message &msg, bool bResult, std::string strMsg)
{
	msg.set_sequence(1);
	pbmsg::Respond *msgRs = msg.mutable_respond();
	msgRs->set_result(bResult);
	msgRs->set_errordescribe(strMsg);
	msgRs->set_lastrespond(true);
	return msgRs;
}

std::string HandleRespondMessage::BuildLoginRespond(bool bResult, std::string strMsg)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::ELoginRq);
	pbmsg::Respond *msgRs = BuildRespondMessage(msg, bResult, strMsg);
	pbmsg::LoginRs *msgLoginRs = msgRs->mutable_login();
	msgLoginRs->set_ttl(1);
	return msg.SerializeAsString();
}
