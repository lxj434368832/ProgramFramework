#pragma once

#include <set>
#include "IMessageHandle.h"

class PbMessageHandle;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainClient *pMain = nullptr);
	~MessageHandle();
	PbMessageHandle* GetProtobufMsgFactory() override;
	HandleRequestMessage* GetHandleRequestMessage() override;
	HandleRespondMessage* GetHandleRespondMessage() override;
	HandleNotifyMessage* GetHandleNotifyMessage() override;


	bool Initialize() override;
	void Uninitialize() override;

	void RegisterMessageHandle() override;
	//处理protobuf数据，供通讯模块调用
	void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	PbMessageHandle				*m_pPbMessageHandle;
	HandleRequestMessage			*m_pHandleRqMsg;
	HandleRespondMessage			*m_pHandleRsMsg;
	HandleNotifyMessage				*m_pHandleNtMsg;
};

