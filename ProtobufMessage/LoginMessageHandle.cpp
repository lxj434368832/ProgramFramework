#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"

LoginMessageHandle::LoginMessageHandle()
{
	ProtobufMsgFactory::instance()->RegisterMessageFunction(pbmsg::MSG::Login_Request,
		std::bind(&LoginMessageHandle::HandleLoginRequest, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));;
}

LoginMessageHandle::~LoginMessageHandle()
{

}

void LoginMessageHandle::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)
{

}
