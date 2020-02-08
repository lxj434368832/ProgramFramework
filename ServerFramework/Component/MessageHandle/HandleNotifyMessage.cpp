#include "HandleNotifyMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"


HandleNotifyMessage::HandleNotifyMessage(IMessageHandle *pMsgHandle)
:HandleMessageColleague(pMsgHandle)
{
	PbMessageHandle *pMsgFctry = pMsgHandle->GetPbMessageHandle();
	//pMsgFctry->RegisterMessageFunction(pbmsg::EMsg::EMessageNt, std::bind(&HandleNotifyMessage::HandleWelcomeNotify,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleNotifyMessage::~HandleNotifyMessage()
{
}

std::string HandleNotifyMessage::BuildWelcomeNotify(std::string strText)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::EMsg::EMessageNt);
	msg.set_sequence(1);
	pbmsg::Notify *msgNt = msg.mutable_notify();
	pbmsg::MessageNt *msgWel = msgNt->mutable_message();
	msgWel->set_text(strText);
	return msg.SerializeAsString();
}

void HandleNotifyMessage::HandleWelcomeNotify(const unsigned uUserKey, const pbmsg::Message *msg)
{
	if (pbmsg::EMsg::EMessageNt != msg->msgtype())
	{
		loge() << "消息类型:" << msg->msgtype() << "错误！";
	}
	const pbmsg::MessageNt &msgWel = msg->notify().message();
}
