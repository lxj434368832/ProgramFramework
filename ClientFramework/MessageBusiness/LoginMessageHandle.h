#pragma once
#include "Message.pb.h"
#include "IMessageHandle.h"

class MessageBusiness;

class LoginMessageHandle : public IMessageHandle
{
public:
	LoginMessageHandle(MessageBusiness *pBusiness);
	~LoginMessageHandle();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);

};
