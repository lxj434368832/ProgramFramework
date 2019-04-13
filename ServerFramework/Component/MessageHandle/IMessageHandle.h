#pragma once
/*************************************************************************
* function：消息处理接口基类，所有需要处理消息的类都继承此类
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/
class IMessageBusiness;

class IMessageHandle
{
public:
	IMessageHandle(IMessageBusiness *pBusiness = nullptr){
		m_pMsgBusiness = pBusiness;
	}
	virtual ~IMessageHandle(){
		m_pMsgBusiness = nullptr;
	}

protected:
	IMessageBusiness *m_pMsgBusiness;
};