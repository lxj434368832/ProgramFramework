#pragma once

#include <string>
#include "IMessageColleague.h"

namespace pbmsg{class Message;}
class MessageHandle;

class HandleNotifyMessage : public IMessageColleague
{
public:
	HandleNotifyMessage(IMessageHandle*);
	~HandleNotifyMessage();
	
	std::string BuildWelcomeNotify(std::string strText);

private:
	void HandleWelcomeNotify(const unsigned uUserKey, const pbmsg::Message *msg);
};

