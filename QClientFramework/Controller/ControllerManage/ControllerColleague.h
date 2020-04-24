#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: 控制器同盟，用于各个子控制器继承
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QObject>

class IModelManage;
class ControllerManage;
class IMainClient;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(ControllerManage*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_pMain = nullptr;
	ControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModel = nullptr; 
};
