#pragma once

#include <QSet>
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include "ModelColleague.h"

class ServerConnect;
class MainController;

class MainModel : public ModelColleague
{
	Q_OBJECT
public:
	MainModel(IModelManage*);
	~MainModel() override;

    bool Initialize();
	void Uninitialize();

	void SaveUserInfo(SUserInfo& user);

	SUserInfo& GetUserInfo();

	/*************************************************************************
	* function����סUserKey��Ӧ����Ϣ
	*************************************************************************/
	void LockUserInfo();

	/*************************************************************************
	* function������UserKey��Ӧ����Ϣ
	*************************************************************************/
	void UnlockUserInfo();

	void AddLoginServer(UserKey uUserKey);

	void DelLoginServer(UserKey uUserKey);

	QSet<unsigned> GetLoginServerList();

private:
	SUserInfo		m_user;
	MLock			m_lckUserInfo;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};
