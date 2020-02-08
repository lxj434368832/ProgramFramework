#pragma once

#include "..\..\CommonFile\TypeDefine.h"

class MainModel
{
public:
    MainModel();

    bool Start();
    void Stop();

	SUserInfo& GetUserInfo();

	void SaveUserInfo(SUserInfo& user);

private:
	SUserInfo	m_user;

};
