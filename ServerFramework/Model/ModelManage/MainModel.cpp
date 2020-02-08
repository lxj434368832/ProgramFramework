#include "MainModel.h"

MainModel::MainModel()
{

}

bool MainModel::Start()
{
    return true;
}

void MainModel::Stop()
{

}

SUserInfo& MainModel::GetUserInfo()
{
	return m_user;
}

void MainModel::SaveUserInfo(SUserInfo& user)
{
	m_user = user;
}
