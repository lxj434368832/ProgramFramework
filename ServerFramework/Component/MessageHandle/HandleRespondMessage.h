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

	std::string BuildLoginRespond(bool bResult, std::string strMsg);

private:
};
