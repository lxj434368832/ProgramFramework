#include "../../CommonFile/CommonDefine.h"
#include "MessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "HandleRequestMessage.h"
#include "../../MainServer/MainServer.h"


MessageHandle::MessageHandle(IMainServer *srv)
	:IMessageHandle(srv)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_setMessageHandle)
	{
		delete value;
	}

	m_setMessageHandle.clear();
}

ProtobufMsgFactory* MessageHandle::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageHandle::Start()
{
	LoadMessageHandleModule();
	return m_pProtoMsgFtry->Start(m_pSrv->GetServerConfig()->uMessageThreadCount);
}

void MessageHandle::Stop()
{
	m_pProtoMsgFtry->Stop();
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, const char * data, unsigned length)
{
	m_pProtoMsgFtry->AddMessageData(uUserKey, data, length);
}

void MessageHandle::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new HandleRequestMessage(this));
}
