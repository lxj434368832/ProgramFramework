#pragma once
/*************************************************************************
* function：消息处理接口基类，所有需要处理消息的类都继承此类
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/
class IMessage;

class IMessageHandle
{
public:
	IMessageHandle(IMessage *pMsg = nullptr){
		m_pMsgModule = pMsg;
	}
	virtual ~IMessageHandle(){
		m_pMsgModule = nullptr;
	}

protected:
	IMessage *m_pMsgModule;
};