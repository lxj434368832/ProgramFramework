#include "MessageHandle.h"
#include "../../MainServer/MainServer.h"
#include "ProtobufMsgFactory.h"
#include "HandleRequestMessage.h"
#include "HandleRespondMessage.h"
#include "HandleNotifyMessage.h"
#include "../../CommonFile/CommonDefine.h"


MessageHandle::MessageHandle(IMainServer *pMain)
	:IMessageHandle(pMain)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;

	m_pHandleRqMsg = nullptr;
	m_pHandleRsMsg = nullptr;
	m_pHandleNtMsg = nullptr;
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pHandleNtMsg);
}

HandleRequestMessage * MessageHandle::GetHandleRequestMessage()
{
	return m_pHandleRqMsg;
}

HandleRespondMessage * MessageHandle::GetHandleRespondMessage()
{
	return m_pHandleRsMsg;
}

HandleNotifyMessage * MessageHandle::GetHandleNotifyMessage()
{
	return m_pHandleNtMsg;
}

ProtobufMsgFactory* MessageHandle::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageHandle::Start()
{
	m_pHandleRqMsg = new HandleRequestMessage(this);
	m_pHandleRsMsg = new HandleRespondMessage(this);
	m_pHandleNtMsg = new HandleNotifyMessage(this);

	return m_pProtoMsgFtry->Start(m_pMain->GetServerConfig()->uMessageThreadCount);
}

void MessageHandle::Stop()
{
	m_pProtoMsgFtry->Stop();
	RELEASE(m_pHandleRsMsg);
	RELEASE(m_pHandleRqMsg);
	RELEASE(m_pProtoMsgFtry);
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, const char * data, unsigned length)
{
	m_pProtoMsgFtry->AddMessageData(uUserKey, data, length);
}
