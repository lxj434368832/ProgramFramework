#include "MainModel.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "../../Component/TCPCommunication/Communication.h"
#include "..\..\Component\MessageHandle\MessageHandle.h"

MainModel::MainModel(ModelManage* pMdlMng) :
ModelColleague(pMdlMng)
{
	connect(m_pCmmnt, SIGNAL(signalTcpConnectNotify(uint, bool)), SLOT(slotTcpConnectNotify(uint, bool)));
	connect(m_pCmmnt, SIGNAL(signalTcpDisconnectNotify(uint)), SLOT(slotTcpDisconnectNotify(uint)));

}

MainModel::~MainModel()
{

}

bool MainModel::Initialize()
{
    return true;
}

void MainModel::Uninitialize()
{

}

void MainModel::LockUserInfo()
{
	m_lckUserInfo.lock();
}

void MainModel::UnlockUserInfo()
{
	m_lckUserInfo.unlock();
}

void MainModel::AddLoginServer(UserKey uUserKey)
{
	m_lckSrvUser.lock();
	m_setSrvUser.insert(uUserKey);
	m_lckSrvUser.unlock();
}

void MainModel::DelLoginServer(UserKey uUserKey)
{
	m_lckSrvUser.lock();
	m_setSrvUser.remove(uUserKey);
	m_lckSrvUser.unlock();
}

QSet<unsigned> MainModel::GetLoginServerList()
{
	return m_setSrvUser;
}

void MainModel::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{

}

void MainModel::slotTcpDisconnectNotify(unsigned uServerType)
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
