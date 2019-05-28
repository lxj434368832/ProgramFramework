#pragma once

class IMainClient;
class ProtobufMsgFactory;
class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;

class IMessageHandle
{
public:
	IMessageHandle(IMainClient *pMain = nullptr):m_pMain(pMain){}
	virtual ~IMessageHandle() { m_pMain = nullptr; }
	inline IMainClient* GetMainClient() { return m_pMain; }
	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;
	virtual HandleRequestMessage* GetHandleRequestMessage() = 0;
	virtual HandleRespondMessage* GetHandleRespondMessage() = 0;
	virtual HandleNotifyMessage* GetHandleNotifyMessage() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

	//处理protobuf数据
	virtual void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) = 0;

protected:
	IMainClient*	m_pMain;
};
