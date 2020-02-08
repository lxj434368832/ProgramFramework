#pragma once

#include <string>
#include "HandleMessageColleague.h"

namespace pbmsg { class Message; }
class MessageHandle;

class HandleRequestMessage : public HandleMessageColleague
{
public:
	HandleRequestMessage(IMessageHandle*);
	~HandleRequestMessage();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message *msg);

};
