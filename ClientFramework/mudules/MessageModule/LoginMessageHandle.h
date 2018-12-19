#pragma once
#include "Message.pb.h"
#include "IMessageHandle.h"

class MessageModule;

class LoginMessageHandle : public IMessageHandle
{
public:
	LoginMessageHandle(MessageModule *pBusiness);
	~LoginMessageHandle();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);

};
