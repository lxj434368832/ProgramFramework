#include "MessageHandle.h"
#include "../../MainClient/IMainClient.h"
#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include <../../3rdParty/Protobuf/include/Message.pb.h>


MessageHandle::MessageHandle(void *pMain)
	:IMessageHandle(pMain)
{
	m_pPbMsgHandle = new PbMessageHandle;

}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pPbMsgHandle);
}

bool MessageHandle::Initialize(unsigned uThreadCount)
{
	logm() << "************开始消息处理的初始化************";
	if (false == m_pPbMsgHandle->Initialize(uThreadCount)) return false;
	return true;
}

void MessageHandle::Uninitialize()
{
	m_pPbMsgHandle->Uninitialize();
}

void MessageHandle::RegisterPbMsgFunction(unsigned msgType, funMessageHandle handle)
{
	if (m_pPbMsgHandle) m_pPbMsgHandle->RegisterPbMsgFunction(msgType, handle);
}

void MessageHandle::RemovePbMsgFunction(unsigned msgType)
{
	if (m_pPbMsgHandle) m_pPbMsgHandle->RemovePbMsgFunction(msgType);
}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	if (m_pPbMsgHandle)
		m_pPbMsgHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
