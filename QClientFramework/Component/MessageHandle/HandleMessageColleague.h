#pragma once
/*************************************************************************
* function：消息处理同盟类，所有需要处理消息的类都继承此类,主要用于存储需要的外部类
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/

class IMessageHandle;
class ICommunication;
class MainController;

class HandleMessageColleague
{
public:
	HandleMessageColleague(IMessageHandle *pHandle = nullptr);
	virtual ~HandleMessageColleague();

	bool Initialize();
	void Uninitialize();

protected:
	IMessageHandle		*m_pMsgHandle = nullptr;
	ICommunication	*m_pTcpCmmnt = nullptr;
	MainController		*m_pMainCtrl = nullptr;		//主控制器
};