#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: 控制器同盟，用于各个子控制器继承
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;
class ITCPCommunication;
class IMainServer;
class IControllerManage;
class IModelManage;

class ControllerColleague
{
public:
    ControllerColleague(IControllerManage*);
    virtual ~ControllerColleague();

protected:
	IMainServer             *m_pMain = nullptr;
    IControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModelMng = nullptr;
	ITCPCommunication		*m_pTcpCmmnt = nullptr;
	HandleRequestMessage	*m_pHandleRqMsg = nullptr;
	HandleRespondMessage	*m_pHandleRsMsg = nullptr;
	HandleNotifyMessage		*m_pHandleNtMsg = nullptr;
};
