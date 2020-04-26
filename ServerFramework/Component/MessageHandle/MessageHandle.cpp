#include "MessageHandle.h"
#include "../../MainServer/IMainServer.h"
#include "PbMessageHandle.h"
#include "HandleRequestMessage.h"
#include "HandleRespondMessage.h"
#include "HandleNotifyMessage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"


MessageHandle::MessageHandle(IMainServer *pMain)
	:IMessageHandle(pMain)
{
	m_pPbMsgHandle = new PbMessageHandle;

	m_pHandleRqMsg = new HandleRequestMessage(this);
	m_pHandleRsMsg = new HandleRespondMessage(this);
	m_pHandleNtMsg = new HandleNotifyMessage(this);
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pHandleNtMsg);
	RELEASE(m_pHandleRsMsg);
	RELEASE(m_pHandleRqMsg);
	RELEASE(m_pPbMsgHandle);
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

PbMessageHandle* MessageHandle::GetPbMessageHandle()
{
	return m_pPbMsgHandle;
}

bool MessageHandle::Initialize()
{
	logm() << "************开始消息处理的初始化************";
	if (false == m_pPbMsgHandle->Initialize(m_pMain->GetServerConfig()->uMessageThreadCount)) return false;
	if (false == m_pHandleRqMsg->Initialize()) return false;
	if (false == m_pHandleNtMsg->Initialize())	return false;
	if (false == m_pHandleRsMsg->Initialize())	return false;
	return true;
}

void MessageHandle::Uninitialize()
{
	m_pPbMsgHandle->Uninitialize();
	m_pHandleRqMsg->Uninitialize();
	m_pHandleRsMsg->Uninitialize();
	m_pHandleNtMsg->Uninitialize();
}

void MessageHandle::RegisterMessageHandle(unsigned msgType, std::function<void(const unsigned uUserKey, SDataExchange*)> fun)
{
	m_pPbMsgHandle->RegisterMessageFunction(msgType, fun);
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	m_pPbMsgHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
