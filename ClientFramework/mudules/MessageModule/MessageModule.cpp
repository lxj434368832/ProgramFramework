#include "../../include/CommonDefine.h"
#include "MessageModule.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"
#include "../../MainClient/MainClient.h"
#include "../../include/TypeDefine.h"


MessageModule::MessageModule(IMainClient *main)
	:IMessage(main)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
}

MessageModule::~MessageModule()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_setMessageHandle)
	{
		delete value;
	}

	m_setMessageHandle.clear();
}

ProtobufMsgFactory * MessageModule::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageModule::Start()
{
	LoadMessageHandleModule();
	return m_pProtoMsgFtry->Start(m_main->GetClientConfig()->uMessageThreadCount);
}

void MessageModule::Stop()
{
	m_pProtoMsgFtry->Stop();
}

void MessageModule::HandleProtobufMessage(unsigned uUserKey, const char * data, unsigned length)
{
	m_pProtoMsgFtry->AddMessageData(uUserKey, data, length);
}

void MessageModule::SendLoginMessage()
{
}

void MessageModule::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new LoginMessageHandle(this));
}
