#include "../../CommonFile/CommonDefine.h"
#include "MessageBusiness.h"
#include "ProtobufMsgFactory.h"
#include "LoginMessageHandle.h"
#include "../../MainServer/MainServer.h"


MessageBusiness::MessageBusiness(IMainServer *srv)
	:IMessageBusiness(srv)
{
	m_pProtoMsgFtry = new ProtobufMsgFactory;
}

MessageBusiness::~MessageBusiness()
{
	RELEASE(m_pProtoMsgFtry);

	for (auto value : m_setMessageHandle)
	{
		delete value;
	}

	m_setMessageHandle.clear();

	google::protobuf::ShutdownProtobufLibrary();

}
ProtobufMsgFactory* MessageBusiness::GetProtobufMsgFactory()
{
	return m_pProtoMsgFtry;
}

bool MessageBusiness::Start()
{
	LoadMessageHandleModule();
	return m_pProtoMsgFtry->Start(m_pSrv->GetServerConfig()->uMessageThreadCount);
}

void MessageBusiness::Stop()
{
	m_pProtoMsgFtry->Stop();
}

void MessageBusiness::HandleProtobufMessage(unsigned uUserKey, const char * data, unsigned length)
{
	m_pProtoMsgFtry->AddMessageData(uUserKey, data, length);
}

void MessageBusiness::LoadMessageHandleModule()
{
	m_setMessageHandle.emplace(new LoginMessageHandle(this));
}
