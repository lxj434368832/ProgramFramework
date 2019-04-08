#ifndef VIEWCOLLEAGUE_H
#define VIEWCOLLEAGUE_H

class ViewMediator;
class IMainClient;
class IControllerManage;

#define StatisticCtrl m_controller->GetStatisticController()

class ViewColleague
{
public:
    ViewColleague(ViewMediator* mdt);
    ~ViewColleague();

protected:
    ViewMediator        *m_mediator;
    IMainClient             *m_mainClient;
    IControllerManage	*m_controller;
};

#endif // VIEWCOLLEAGUE_H
