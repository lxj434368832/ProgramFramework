#pragma once
#include "Message.pb.h"
#include "IMessageHandle.h"

class MessageModule;

class LoginMessageHandle : public IMessageHandle
{
public:
	LoginMessageHandle(MessageModule *pMsg);
	~LoginMessageHandle();
	void SendLoginMessage();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);
};
