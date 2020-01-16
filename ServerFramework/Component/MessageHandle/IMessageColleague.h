#pragma once
/*************************************************************************
* function：消息处理同盟类，所有需要处理消息的类都继承此类,主要用于存储需要的外部类
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/

class IMessageHandle;
class ITCPCommunication;
class MainController;

class IMessageColleague
{
public:
	IMessageColleague(IMessageHandle *pHandle = nullptr);
	virtual ~IMessageColleague();

	bool Start();
	void Stop();

protected:
	IMessageHandle		*m_pMsgHandle;
	ITCPCommunication	*m_pTcpCmmnt;
	MainController		*m_pMainCtrl;
};