#include "ModelColleague.h"
#include "ModelManage.h"


ModelColleague::ModelColleague(IModelManage *pMdlMng, QObject *parent)
	:QObject(parent)
{
	m_pMain = pMdlMng->GetMainClient();
	m_pMdlMng = pMdlMng;
	m_pCmmnt = pMdlMng->GetCommunication();
	m_pMsgHandle = pMdlMng->GetMessageHandle();
}

ModelColleague::~ModelColleague()
{
	m_pMain = nullptr;
	m_pMdlMng = nullptr;
	m_pCmmnt = nullptr;
	m_pMsgHandle = nullptr;
}
