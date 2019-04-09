#pragma once

#include <map>
#include "IMessage.h"

class ProtobufMsgFactory;
class IMessageHandle;

class MessageHandle : public IMessage
{
public:
	MessageHandle(IMainClient *main = nullptr);
	~MessageHandle();
	ProtobufMsgFactory* GetProtobufMsgFactory() override;
	LoginMessageHandle* GetLoginMessageHandle() override;
	bool Start() override;
	void Stop() override;
	//´¦protobufÊý¾Ý
	void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) override;

private:
	void LoadMessageHandleModule();

private:
	enum EMessageHandleModule
	{
		EMHM_UNKNOWN,
		EMHM_LOGIN
	};

	ProtobufMsgFactory			*m_pProtoMsgFtry;
	std::map<EMessageHandleModule,IMessageHandle*>	m_mapMessageHandle;

};

