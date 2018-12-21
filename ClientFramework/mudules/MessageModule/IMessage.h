#pragma once

class IMainClient;
class ProtobufMsgFactory;

class IMessage
{
public:
	IMessage(IMainClient *main = nullptr):m_main(main){ }
	virtual ~IMessage() { m_main = nullptr; };
	inline IMainClient* GetMainClient() { return m_main; }

	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;
	virtual bool Start() = 0;
	virtual void Stop() = 0;

	//´¦protobufÊý¾Ý
	virtual void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) = 0;
	virtual void SendLoginMessage() = 0;

protected:
	IMainClient *m_main;
};
