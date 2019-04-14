#pragma once
/*************************************************************************
* function：对外的通讯服务的接口，所有的通讯包括数据库交互都在此模块中管理
* author :	明巧文
* datetime：2017-12-14
* company:
*************************************************************************/

#include "..\..\CommonFile\TypeDefine.h"

class IMainServer;

class ITCPCommunication
{
public:
	ITCPCommunication(IMainServer *pMain = nullptr):m_pMain(pMain){};
	virtual ~ITCPCommunication() { m_pMain = nullptr; };
	inline IMainServer* GetMainServer() { return m_pMain; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;
	virtual void SendData(UserKey uUserKey, const char* data, unsigned uLength) = 0;

protected:
	IMainServer *m_pMain;
};
