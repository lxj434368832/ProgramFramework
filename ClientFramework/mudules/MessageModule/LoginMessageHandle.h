#pragma once

#include "IMessageHandle.h"

class MessageModule;
namespace pbmsg
{
	class Message;
}

class LoginMessageHandle : public IMessageHandle
{
public:
	LoginMessageHandle(MessageModule *pMsg);
	~LoginMessageHandle();
	void SendLoginMessage(unsigned uUserKey);

private:
	void HandleLoginResponse(const unsigned uUserKey, const pbmsg::Message *msg, void* ptr);
};
