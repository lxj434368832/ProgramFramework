#include "TypeDefine.h"

void ClientUserInfo::Reset()
{
	m_uUserKey = 0;
	m_uUserId = 0;
	m_strName.clear();
	m_strPassword.clear();
	m_strSex.clear();
	m_unAge = 0;
	m_strFamilyAddress.clear();
	m_strCompanyAddress.clear();
}
