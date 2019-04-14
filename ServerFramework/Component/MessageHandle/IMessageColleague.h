#pragma once
/*************************************************************************
* function：消息处理同盟类，所有需要处理消息的类都继承此类
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/
class IMessageHandle;

class IMessageColleague
{
public:
	IMessageColleague(IMessageHandle *pHandle = nullptr){
		m_pMsgHandle = pHandle;
	}
	virtual ~IMessageColleague(){
		m_pMsgHandle = nullptr;
	}

protected:
	IMessageHandle *m_pMsgHandle;
};