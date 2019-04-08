#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: 控制器同盟，用于各个子控制器继承
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QObject>

class IMainClient;
class IControllerManage;
class IModelManage;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(IMainClient*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_main = nullptr;
    IControllerManage *m_controller = nullptr;
    IModelManage        *m_model = nullptr;
};
