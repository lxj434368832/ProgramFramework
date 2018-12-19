#pragma once

#include <set>
#include "IMessage.h"

class ProtobufMsgFactory;
class IMessageHandle;

class MessageModule : public IMessage
{
public:
	MessageModule(IMainClient *main = nullptr);
	~MessageModule();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	bool Start() override;
	void Stop() override;

private:
	void LoadMessageHandleModule();

private:
	ProtobufMsgFactory			*m_pProtoMsgFtry;
	std::set<IMessageHandle*>	m_setMessageHandle;

};

