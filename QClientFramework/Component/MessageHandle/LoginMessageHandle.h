#pragma once

#include "IMessageHandle.h"

class MessageHandle;
namespace pbmsg
{
	class Message;
}

class LoginMessageHandle : public IMessageHandle
{
public:
	LoginMessageHandle(MessageHandle *pMsg);
	~LoginMessageHandle();
	void SendLoginMessage(unsigned uUserKey);

private:
	void HandleLoginResponse(const unsigned uUserKey, const pbmsg::Message *msg, void* ptr);
};
