#pragma once

#include <set>
#include "IMessageHandle.h"

class ProtobufMsgFactory;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainClient *pMain = nullptr);
	~MessageHandle();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	HandleRequestMessage* GetHandleRequestMessage() override;
	HandleRespondMessage* GetHandleRespondMessage() override;
	HandleNotifyMessage* GetHandleNotifyMessage() override;


	bool Start() override;
	void Stop() override;

	//处理protobuf数据，供通讯模块调用
	void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) override;

private:
	ProtobufMsgFactory				*m_pProtoMsgFtry;
	HandleRequestMessage			*m_pHandleRqMsg;
	HandleRespondMessage			*m_pHandleRsMsg;
	HandleNotifyMessage				*m_pHandleNtMsg;
};

