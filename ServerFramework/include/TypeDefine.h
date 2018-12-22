#pragma once
#include "../../IOCPCommunication/INetInterface.h"

struct ClientUserInfo : UserInfo
{
	std::string	m_strName;
	std::string m_strPassword;
	std::string m_strSex;
	unsigned	m_unAge;
	std::string m_strFamilyAddress;
	std::string m_strCompanyAddress;

	void Reset()
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
};
