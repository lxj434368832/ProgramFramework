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

	std::string BuildLoginRespond(bool bResult, std::string strMsg);

private:
};
