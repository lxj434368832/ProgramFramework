#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: 控制器同盟，用于各个子控制器继承
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

class IMainClient;
class IControllerManage;
class IModelManage;

class ControllerColleague
{
public:
	ControllerColleague(IControllerManage* pCtrlMng);
    ~ControllerColleague();

protected:
    IMainClient             *m_pMain = nullptr;
	IControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModelMng = nullptr;
};
