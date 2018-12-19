#include "../../include/CommonDefine.h"
#include "MessageModule.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"
#include "../../MainClient/MainClient.h"


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
ProtobufMsgFactory* MessageModule::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageModule::Start()
{
	LoadMessageHandleModule();
	//return m_pProtoMsgFtry->Start(m_pSrv->GetServerConfig()->uMessageThreadCount);
}

void MessageModule::Stop()
{
	m_pProtoMsgFtry->Stop();
}

void MessageModule::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new LoginMessageHandle(this));
}
