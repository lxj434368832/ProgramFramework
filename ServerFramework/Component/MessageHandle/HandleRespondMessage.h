#pragma once
#include "Message.pb.h"
#include "IMessageColleague.h"

class MessageHandle;

class HandleRespondMessage : public IMessageColleague
{
public:
	HandleRespondMessage(IMessageHandle*);
	~HandleRespondMessage();

	std::string BuildLoginRespond(bool bResult, std::string strMsg);

private:
	void HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);
};

