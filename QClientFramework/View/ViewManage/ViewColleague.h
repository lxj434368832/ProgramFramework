#ifndef VIEWCOLLEAGUE_H
#define VIEWCOLLEAGUE_H

class ViewMediator;
class IMainClient;
class IControllerManage;

//#include "../../Controller/ControllerManage/IControllerManage.h"
#define MainCtrl m_pCtrlMng->GetMainController()        //MainController.h

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
