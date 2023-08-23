#include "HandleNotifyMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include <Message.pb.h>

HandleNotifyMessage::HandleNotifyMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{
	PbMessageHandle *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	//pMsgFctry->RegisterMessageFunction(pbmsg::MSG::EWelcomeNotify, std::bind(&HandleNotifyMessage::HandleWelcomeNotify,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleNotifyMessage::~HandleNotifyMessage()
{
}

std::string HandleNotifyMessage::BuildWelcomeNotify(std::string strText)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::EMessageNt);
	//msg.set_sequence(1);
	//pbmsg::Notify *msgNt = msg.mutable_notify();
	//pbmsg::WelcomeNotify *msgWel = msgNt->mutable_welcome();
	//msgWel->set_text(strText);
	return msg.SerializeAsString();
}

void HandleNotifyMessage::HandleWelcomeNotify(const unsigned uUserKey, const pbmsg::Message *msg)
{
	//if (pbmsg::MSG::EWelcomeNotify != msg->msgtype())
	//{
	//	loge() << "消息类型:" << msg->msgtype() << "错误！";
	//}
	//const pbmsg::WelcomeNotify &msgWel = msg->notify().welcome();
}
