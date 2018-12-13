#pragma once

class ProtobufMsgFactory;
class IMessageHandle;

class MessageBusiness
{
public:
	MessageBusiness(void *facade);
	~MessageBusiness();
	inline ProtobufMsgFactory* GetProtobufMsgFactory(){ return m_pProtoMsgFtry; }

	void StartBusiness();
	void StopBusiness();

private:
	void LoadMessageHandleModule();

private:
	void*		m_facade;
	ProtobufMsgFactory			*m_pProtoMsgFtry;
	std::set<IMessageHandle*>	m_setMessageHandle;
};

