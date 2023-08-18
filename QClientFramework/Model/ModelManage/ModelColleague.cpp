#include "ModelColleague.h"
#include "ModelManage.h"


ModelColleague::ModelColleague(IModelManage *pMdlMng, QObject *parent)
	:QObject(parent)
{
	m_pMain = pMdlMng->GetMainClient();
	m_pMdlMng = pMdlMng;
}

ModelColleague::~ModelColleague()
{
	m_pMain = nullptr;
	m_pMdlMng = nullptr;
}
