#pragma once
#include <string>
#include "IMessageColleague.h"

namespace pbmsg { class Message; }
class MessageHandle;

class HandleRespondMessage : public IMessageColleague
{
public:
	HandleRespondMessage(IMessageHandle*);
	~HandleRespondMessage();

private:
	void HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message *msg);
};
