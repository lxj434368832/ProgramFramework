#pragma once

#include <set>
#include "IMessageHandle.h"

class ProtobufMsgFactory;
class IMessageColleague;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainServer *srv = nullptr);
	~MessageHandle();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	bool Start() override;
	void Stop() override;

	//´¦protobufÊý¾Ý
	void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) override;

private:
	void LoadMessageHandleModule();

private:
	ProtobufMsgFactory				*m_pProtoMsgFtry;
	std::set<IMessageColleague*>	m_setMessageHandle;

};

