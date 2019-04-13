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

	void Reset();
};
