#include "ModelColleague.h"
#include "../include/IModelManage.h"


ModelColleague::ModelColleague(IModelManage *pMdlMng)
{
	m_pMain = pMdlMng->GetMainClient();
	m_pMdlMng = pMdlMng;
}

ModelColleague::~ModelColleague()
{
	m_pMain = nullptr;
	m_pMdlMng = nullptr;
}
