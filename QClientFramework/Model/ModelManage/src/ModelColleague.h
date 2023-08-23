#pragma once
/************************************************************************
* filename:ModelColleague.h
* function: 模型同盟，用于各个子控制器继承
* author :	明巧文
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
