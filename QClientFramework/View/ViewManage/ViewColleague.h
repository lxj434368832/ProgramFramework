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
    ViewMediator        *m_pMdt;
    IMainClient             *m_pMain;
    IControllerManage	*m_pCtrlMng;
};

#endif // VIEWCOLLEAGUE_H
