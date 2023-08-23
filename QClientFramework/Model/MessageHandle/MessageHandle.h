#pragma once

#include "IMessageHandle.h"

class PbMessageHandle;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(void *pMain = nullptr);
	~MessageHandle();

	bool Initialize(unsigned uThreadCount) override;
	void Uninitialize() override;

	void RegisterPbMsgFunction(unsigned, funMessageHandle) override;
	void RemovePbMsgFunction(unsigned msgType) override;

	//处理protobuf数据，供通讯模块调用
	void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	PbMessageHandle				*m_pPbMsgHandle;
};

