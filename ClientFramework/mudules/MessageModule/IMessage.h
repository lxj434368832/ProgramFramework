#pragma once

class IMainClient;
class ProtobufMsgFactory;

class IMessage
{
public:
	IMessage(IMainClient *main = nullptr):m_main(main){ }
	virtual ~IMessage() { m_main = nullptr; };
	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:
	IMainClient *m_main;
};
