#pragma once
/*************************************************************************
* function：对外的管理服务的接口，主要对所有的数据进行存储管理
* author :	明巧文
* datetime：2018-12-14
* company:
*************************************************************************/
class IMainServer;
class UserInfoManage;

class IModelManage
{
public:
	IModelManage(IMainServer* srv = nullptr):m_pSrv(srv){}
	virtual ~IModelManage() { m_pSrv = nullptr; }
	virtual IMainServer* GetMainServer() { return m_pSrv; }

	virtual UserInfoManage* GetUserInfoManage() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainServer*	m_pSrv;

};
