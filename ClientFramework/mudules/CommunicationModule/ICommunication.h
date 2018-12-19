#pragma once
/*************************************************************************
* function：对外的通讯服务的接口，所有的通讯包括数据库交互都在此模块中管理
* author :	明巧文
* datetime：2017-12-14
* company:
*************************************************************************/

class IMainClient;

class ICommunication
{
public:
	ICommunication(IMainClient *main = nullptr):m_main(main){};
	virtual ~ICommunication() { m_main = nullptr; };

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainClient *m_main;
};
