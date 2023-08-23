#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
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
