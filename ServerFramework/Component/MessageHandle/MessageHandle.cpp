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

	m_pHandleRqMsg = new HandleRequestMessage(this);
	m_pHandleRsMsg = new HandleRespondMessage(this);
	m_pHandleNtMsg = new HandleNotifyMessage(this);
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pHandleNtMsg);
	RELEASE(m_pHandleRsMsg);
	RELEASE(m_pHandleRqMsg);
	RELEASE(m_pProtoMsgFtry);
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
	logm() << "************开始消息处理的初始化************";
	if (false == m_pProtoMsgFtry->Start(m_pMain->GetServerConfig()->uMessageThreadCount)) return false;
	if (false == m_pHandleRqMsg->Start()) return false;
	if (false == m_pHandleNtMsg->Start())	return false;
	if (false == m_pHandleRsMsg->Start())	return false;
	return true;
}

void MessageHandle::Stop()
{
	m_pProtoMsgFtry->Stop();
	m_pHandleRqMsg->Stop();
	m_pHandleRsMsg->Stop();
	m_pHandleNtMsg->Stop();
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, const char * data, unsigned length)
{
	m_pProtoMsgFtry->AddMessageData(uUserKey, data, length);
}
