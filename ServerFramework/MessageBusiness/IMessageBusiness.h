#pragma once

class IMainServer;
class ProtobufMsgFactory;

class IMessageBusiness
{
public:
	IMessageBusiness(IMainServer *srv = nullptr):m_pSrv(srv){}
	virtual ~IMessageBusiness() { m_pSrv = nullptr; }
	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainServer*	m_pSrv;
};
