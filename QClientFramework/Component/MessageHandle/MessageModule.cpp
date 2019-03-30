#include "../../CommonFile/CommonDefine.h"
#include "MessageModule.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"
#include "../../MainClient/MainClient.h"
#include "../../CommonFile/TypeDefine.h"

MessageModule::MessageModule(IMainClient *main)
	:IMessage(main)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
	LoadMessageHandleModule();
}

MessageModule::~MessageModule()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_mapMessageHandle)
	{
		delete value.second;
	}

	m_mapMessageHandle.clear();

	google::protobuf::ShutdownProtobufLibrary();
}

ProtobufMsgFactory * MessageModule::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

LoginMessageHandle * MessageModule::GetLoginMessageHandle()
{
	return static_cast<LoginMessageHandle*>(m_mapMessageHandle[EMHM_LOGIN]);
}

bool MessageModule::Start()
{
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

void MessageModule::LoadMessageHandleModule()
{
	m_mapMessageHandle.emplace(EMHM_LOGIN, new LoginMessageHandle(this));
}
