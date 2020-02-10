#pragma once
#include <string>
#include "HandleMessageColleague.h"

namespace pbmsg { class Message; }
class MessageHandle;

class HandleRespondMessage : public HandleMessageColleague
{
public:
	HandleRespondMessage(IMessageHandle*);
	~HandleRespondMessage();

private:
	void HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message *msg);
};
