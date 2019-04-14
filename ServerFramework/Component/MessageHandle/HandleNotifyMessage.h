#pragma once
#include "Message.pb.h"
#include "IMessageColleague.h"

class MessageHandle;

class HandleNotifyMessage : public IMessageColleague
{
public:
	HandleNotifyMessage(IMessageHandle*);
	~HandleNotifyMessage();
	
	std::string BuildWelcomeNotify(std::string strText);

private:
	void HandleWelcomeNotify(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);
};

