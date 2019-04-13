#pragma once

#include <set>
#include "IMessageBusiness.h"

class ProtobufMsgFactory;
class IMessageHandle;

class MessageBusiness : public IMessageBusiness
{
public:
	MessageBusiness(IMainServer *srv = nullptr);
	~MessageBusiness();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	bool Start() override;
	void Stop() override;

	//´¦protobufÊý¾Ý
	void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) override;

private:
	void LoadMessageHandleModule();

private:
	ProtobufMsgFactory			*m_pProtoMsgFtry;
	std::set<IMessageHandle*>	m_setMessageHandle;

};

