#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QObject>

class IMainClient;
class IModelManage;
class ControllerManage;
class IMessageHandle;
class ICommunication;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(ControllerManage*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_pMain = nullptr;
	ControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModel = nullptr; 
	ICommunication*			m_pCmmnt = nullptr;
	IMessageHandle*			m_pMsgHandle = nullptr;
};
