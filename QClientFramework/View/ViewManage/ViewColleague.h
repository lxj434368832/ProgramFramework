#ifndef VIEWCOLLEAGUE_H
#define VIEWCOLLEAGUE_H

class ViewMediator;
class IMainClient;
class ControllerManage;

class ViewColleague
{
public:
    ViewColleague(ViewMediator* mdt);
    ~ViewColleague();

protected:
    ViewMediator        *m_mediator;
    IMainClient         *m_mainClient; 
	ControllerManage			*m_controller;
};

#endif // VIEWCOLLEAGUE_H
