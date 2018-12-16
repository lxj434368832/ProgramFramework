#include "../CommonDefine.h"
#include "MessageBusiness.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"


MessageBusiness::MessageBusiness(IMainServer *srv)
	:IMessageBusiness(srv)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
}

MessageBusiness::~MessageBusiness()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_setMessageHandle)
	{
		delete value;
	}

	m_setMessageHandle.clear();
}
ProtobufMsgFactory* MessageBusiness::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageBusiness::Start()
{
	LoadMessageHandleModule();
	return m_pProtoMsgFtry->Start(10);
}

void MessageBusiness::Stop()
{
	m_pProtoMsgFtry->Stop();
}

void MessageBusiness::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new LoginMessageHandle(this));
}
