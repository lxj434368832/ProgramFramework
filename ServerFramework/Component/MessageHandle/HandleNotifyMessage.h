#pragma once

#include <string>
#include "HandleMessageColleague.h"

namespace pbmsg{class Message;}
class MessageHandle;

class HandleNotifyMessage : public HandleMessageColleague
{
public:
	HandleNotifyMessage(IMessageHandle*);
	~HandleNotifyMessage();
	
	std::string BuildWelcomeNotify(std::string strText);

private:
	void HandleWelcomeNotify(const unsigned uUserKey, const pbmsg::Message *msg);
};

