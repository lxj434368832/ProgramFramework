#pragma once

#include <set>
#include "IMessageHandle.h"

class ProtobufMsgFactory;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainServer *pMain = nullptr);
	~MessageHandle();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	HandleRequestMessage* GetHandleRequestMessage() override;
	HandleRespondMessage* GetHandleRespondMessage() override;
	HandleNotifyMessage* GetHandleNotifyMessage() override;


	bool Start() override;
	void Stop() override;

	//´¦protobufÊý¾Ý
	void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) override;

private:
	ProtobufMsgFactory				*m_pProtoMsgFtry;
	HandleRequestMessage			*m_pHandleRqMsg;
	HandleRespondMessage			*m_pHandleRsMsg;
	HandleNotifyMessage				*m_pHandleNtMsg;
};

