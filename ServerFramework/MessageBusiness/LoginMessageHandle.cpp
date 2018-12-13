#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "MessageBusiness.h"

LoginMessageHandle::LoginMessageHandle(MessageBusiness *pBusiness)
	:IMessageHandle(pBusiness)
{
	ProtobufMsgFactory *pMsgFctry = pBusiness->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::Login_Request,std::bind(&LoginMessageHandle::HandleLoginRequest,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));;
}

LoginMessageHandle::~LoginMessageHandle()
{

}

void LoginMessageHandle::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)
{

}
