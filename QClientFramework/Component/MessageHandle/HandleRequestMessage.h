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
	std::string BuildLoginRequest(std::string strUserName, std::string strPassword);

private:
};
