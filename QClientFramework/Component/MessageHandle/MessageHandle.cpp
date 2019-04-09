#include "../../CommonFile/CommonDefine.h"
#include "MessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"
#include "../../MainClient/MainClient.h"
#include "../../CommonFile/TypeDefine.h"

MessageHandle::MessageHandle(IMainClient *main)
	:IMessage(main)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
	LoadMessageHandleModule();
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_mapMessageHandle)
	{
		delete value.second;
	}

	m_mapMessageHandle.clear();

	google::protobuf::ShutdownProtobufLibrary();
}

ProtobufMsgFactory * MessageHandle::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

LoginMessageHandle * MessageHandle::GetLoginMessageHandle()
{
	return static_cast<LoginMessageHandle*>(m_mapMessageHandle[EMHM_LOGIN]);
}

bool MessageHandle::Start()
{
	return m_pProtoMsgFtry->Start(m_main->GetClientConfig()->uMessageThreadCount);
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
	m_mapMessageHandle.emplace(EMHM_LOGIN, new LoginMessageHandle(this));
}
