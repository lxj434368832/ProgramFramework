#include "HandleRespondMessage.h"
#include "IMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "../../CommonFile/CommonDefine.h"



HandleRespondMessage::HandleRespondMessage(IMessageHandle *pMsgHandle)
{
	ProtobufMsgFactory *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::ELoginRespond, std::bind(&HandleRespondMessage::HandleLoginRespond,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
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

void HandleRespondMessage::HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message & msg, void * ptr)
{
	if (pbmsg::MSG::ELoginRespond != msg.msgtype())
	{
		loge() << "消息类型:" << msg.msgtype() << "错误！";
	}

	pbmsg::Respond msgRs = msg.respond();
	msgRs.result();
	//调用控制层处理登录信息
}
