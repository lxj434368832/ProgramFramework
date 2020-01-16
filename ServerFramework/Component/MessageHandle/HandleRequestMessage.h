#pragma once

#include <string>
#include "IMessageColleague.h"

namespace pbmsg { class Message; }
class MessageHandle;

class HandleRequestMessage : public IMessageColleague
{
public:
	HandleRequestMessage(IMessageHandle*);
	~HandleRequestMessage();

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message *msg);

};
