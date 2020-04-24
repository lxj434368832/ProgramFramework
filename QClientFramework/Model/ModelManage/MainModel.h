#pragma once

#include <QSet>
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include "ModelColleague.h"

class MainModel : public ModelColleague
{
	Q_OBJECT

public:
	MainModel(ModelManage*);
	~MainModel() override;

    bool Initialize();
	void Uninitialize();

	void SaveUserInfo(SUserInfo& user);

	SUserInfo& GetUserInfo();

	/*************************************************************************
	* function：锁住UserKey对应的信息
	*************************************************************************/
	void LockUserInfo();

	/*************************************************************************
	* function：解锁UserKey对应的信息
	*************************************************************************/
	void UnlockUserInfo();

	void AddLoginServer(UserKey uUserKey);

	void DelLoginServer(UserKey uUserKey);

	QSet<unsigned> GetLoginServerList();

private slots:

	/*************************以下为本类自定槽函数*************************/
	/*************************************************************************
	* function：		 Tcp连接状态通知
	* param uServerType: 服务器类型
	* param state:		 连接成功标识
	* param once:		 第一次连接标识
	*************************************************************************/
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
	SUserInfo	m_user;
	MLock		m_lckUserInfo;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};
