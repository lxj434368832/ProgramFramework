#pragma once

class MessageBusiness;

class IMessageHandle
{
public:
	IMessageHandle(MessageBusiness *pBusiness = nullptr){
		m_pMsgBusiness = pBusiness;
	}
	virtual ~IMessageHandle(){
		m_pMsgBusiness = nullptr;
	}

protected:
	MessageBusiness *m_pMsgBusiness;
};