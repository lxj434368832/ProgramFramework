#include "HandleRespondMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Controller/ControllerManage/MainController.h"


HandleRespondMessage::HandleRespondMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{
	PbMessageHandle *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	//pMsgFctry->RegisterMessageFunction(pbmsg::ELoginRs, std::bind(&HandleRespondMessage::HandleLoginRespond,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleRespondMessage::~HandleRespondMessage()
{
}

void HandleRespondMessage::HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message *msg)
{
	if (pbmsg::ELoginRs != msg->msgtype())
	{
		loge() << "��Ϣ����:" << msg->msgtype() << "����";
	}

	const pbmsg::Respond &msgRs = msg->respond();
	//���ÿ��Ʋ㴦���¼��Ϣ
	//m_pMainCtrl->UserLoginResult(uUserKey, msgRs.result(), msgRs.errordescribe());
}
