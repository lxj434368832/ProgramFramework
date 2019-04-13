#pragma once
/*************************************************************************
* function：对外的通讯服务的接口，所有的通讯包括数据库交互都在此模块中管理
* author :	明巧文
* datetime：2017-12-14
* company:
*************************************************************************/

class IMainServer;

class ICommunication
{
public:
	ICommunication(IMainServer *srv = nullptr):m_srv(srv){};
	virtual ~ICommunication() { m_srv = nullptr; };

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainServer *m_srv;
};
