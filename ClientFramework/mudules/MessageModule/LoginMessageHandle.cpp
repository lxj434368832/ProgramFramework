#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "MessageModule.h"
#include "../../MainClient/IMainClient.h"
#include "../CommunicationModule/ICommunication.h"
#include "../CommunicationModule/ServerConnect.h"

LoginMessageHandle::LoginMessageHandle(MessageModule *pMsg)
	:IMessageHandle(pMsg)
{
	ProtobufMsgFactory *pMsgFctry = pMsg->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::Login_Request,std::bind(&LoginMessageHandle::HandleLoginRequest,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));;
}

LoginMessageHandle::~LoginMessageHandle()
{

}

void LoginMessageHandle::SendLoginMessage()
{
	pbmsg::LoginRequest login;
	login.set_username("mingel");
	login.set_password("mingqiaowen");
	pbmsg::Request rq;
	rq.set_allocated_login(&login);
	pbmsg::Message msg;
	msg.set_msg_type(pbmsg::Login_Request);
	msg.set_allocated_request(&rq);
	std::string strMsg = msg.SerializeAsString();
	m_pMsgModule->GetMainClient()->GetMainCommunication()->SendData(EST_CMD_SERVER, strMsg.data(), strMsg.length());
}

void LoginMessageHandle::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)
{

}
