#include"HandleRequestMessage.h"
#include "IMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include "../../Controller/ControllerManage/MainController.h"

HandleRequestMessage::HandleRequestMessage(IMessageHandle *pMsgHandle)
	:IMessageColleague(pMsgHandle)
{
	ProtobufMsgFactory *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::ELoginRequest,std::bind(&HandleRequestMessage::HandleLoginRequest,
		this, std::placeholders::_1, std::placeholders::_2));
}

HandleRequestMessage::~HandleRequestMessage()
{

}

void HandleRequestMessage::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message *msg)
{
	if (pbmsg::MSG::ELoginRequest != msg->msgtype())
	{
		loge() << "消息类型:" << msg->msgtype() << "错误！";
	}
	const pbmsg::LoginRequest &login = msg->request().login();

	ClientUserInfo	userInfo;
	userInfo.m_uUserKey = uUserKey;
	userInfo.m_strName = login.username();
	userInfo.m_strPassword = login.password();

	//调用控制层处理登录信息
	m_pMainCtrl->UserLogin(userInfo);
}
