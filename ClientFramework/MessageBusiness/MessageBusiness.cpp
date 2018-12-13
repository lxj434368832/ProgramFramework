#include "../stdafx.h"
#include "MessageBusiness.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"


MessageBusiness::MessageBusiness(void *facade)
{
	m_facade = facade;
	m_pProtoMsgFtry = new ProtobufMsgFactory;
}

MessageBusiness::~MessageBusiness()
{
	m_facade = nullptr;
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_setMessageHandle)
	{
		delete value;
	}

	m_setMessageHandle.clear();
}

void MessageBusiness::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new LoginMessageHandle(this));
}
