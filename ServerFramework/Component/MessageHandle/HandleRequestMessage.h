#pragma once
#include "Message.pb.h"
#include "IMessageColleague.h"

class MessageHandle;

class HandleRequestMessage : public IMessageColleague
{
public:
	HandleRequestMessage(IMessageHandle*);
	~HandleRequestMessage();
	std::string BuildLoginRequest(std::string strUserName, std::string strPassword);

private:
	void HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr);

};
