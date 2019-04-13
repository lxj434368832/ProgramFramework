#pragma once

class IMainServer;
class ProtobufMsgFactory;

class IMessageBusiness
{
public:
	IMessageBusiness(IMainServer *srv = nullptr):m_pSrv(srv){}
	virtual ~IMessageBusiness() { m_pSrv = nullptr; }
	inline IMainServer* GetMainServer() { return m_pSrv; }
	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

	//´¦protobufÊý¾Ý
	virtual void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) = 0;

protected:
	IMainServer*	m_pSrv;
};
