#include "MessageHandle.h"
#include "../../MainServer/IMainServer.h"
#include "PbMessageHandle.h"
#include "HandleRequestMessage.h"
#include "HandleRespondMessage.h"
#include "HandleNotifyMessage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../../Controller/ControllerManage/MainController.h"
#include <Message.pb.h>


MessageHandle::MessageHandle(IMainServer *pMain)
	:IMessageHandle(pMain)
{
	m_pPbMessageHandle = new PbMessageHandle;

	m_pHandleRqMsg = new HandleRequestMessage(this);
	m_pHandleRsMsg = new HandleRespondMessage(this);
	m_pHandleNtMsg = new HandleNotifyMessage(this);
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pHandleNtMsg);
	RELEASE(m_pHandleRsMsg);
	RELEASE(m_pHandleRqMsg);
	RELEASE(m_pPbMessageHandle);
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
	return m_pPbMessageHandle;
}

bool MessageHandle::Initialize()
{
	logm() << "************开始消息处理的初始化************";
	if (false == m_pPbMessageHandle->Initialize(m_pMain->GetServerConfig()->uMessageThreadCount)) return false;
	if (false == m_pHandleRqMsg->Initialize()) return false;
	if (false == m_pHandleNtMsg->Initialize())	return false;
	if (false == m_pHandleRsMsg->Initialize())	return false;
	return true;
}

void MessageHandle::Uninitialize()
{
	m_pPbMessageHandle->Uninitialize();
	m_pHandleRqMsg->Uninitialize();
	m_pHandleRsMsg->Uninitialize();
	m_pHandleNtMsg->Uninitialize();
}

void MessageHandle::RegisterMessageHandle()
{
	/*************************注册MainController的消息处理*************************/
	MainController *pMainCtrl = m_pMain->GetControllerManage()->GetMainController();
	m_pPbMessageHandle->RegisterMessageFunction(pbmsg::EMsg::ELoginRq, 
		std::bind(&MainController::HandleLoginRq, pMainCtrl, std::placeholders::_1, std::placeholders::_2));
	m_pPbMessageHandle->RegisterMessageFunction(pbmsg::EHeartbeatNt,
		std::bind(&MainController::HandleHeartbeatNt, pMainCtrl, std::placeholders::_1, std::placeholders::_2));


	/*************************注册Controller的消息处理*************************/

}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	m_pPbMessageHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
