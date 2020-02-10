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
	std::string BuildLoginRequest(std::string strUserName, std::string strPassword);

private:
};
