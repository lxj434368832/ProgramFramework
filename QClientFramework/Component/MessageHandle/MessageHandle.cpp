#include "MessageHandle.h"
#include "../../MainClient/IMainClient.h"
#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include <../../3rdParty/Protobuf/include/Message.pb.h>


MessageHandle::MessageHandle(IMainClient *pMain)
	:IMessageHandle(pMain)
{
	m_pPbMessageHandle = new PbMessageHandle;

}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pPbMessageHandle);
}

PbMessageHandle* MessageHandle::GetPbMessageHandle()
{
	return m_pPbMessageHandle;
}

bool MessageHandle::Initialize()
{
	logm() << "************开始消息处理的初始化************";
	if (false == m_pPbMessageHandle->Initialize(
		m_pMain->GetClientConfig()->uMessageThreadCount)) return false;
	return true;
}

void MessageHandle::Uninitialize()
{
	m_pPbMessageHandle->Uninitialize();
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	m_pPbMessageHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
