#pragma once
#include "Message.pb.h"

class LoginMessageHandle
{
public:
	LoginMessageHandle();
	~LoginMessageHandle();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);

};
