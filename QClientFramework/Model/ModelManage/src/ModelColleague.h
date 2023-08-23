#pragma once
/************************************************************************
* filename:ModelColleague.h
* function: ģ��ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2020-4-24
* company:
/************************************************************************/

#include <QObject>

class IModelManage;
class IMainClient;

class ModelColleague
{
public:
	ModelColleague(IModelManage *);
	virtual ~ModelColleague();

protected:
	IMainClient             *m_pMain = nullptr;
	IModelManage			*m_pMdlMng = nullptr;
};
