#pragma once
/*************************************************************************
* function：对外的通讯服务的接口，所有的通讯包括数据库交互都在此模块中管理
* author :	明巧文
* datetime：2017-12-14
* company:
*************************************************************************/

#include <functional>
#include "..\CommonFile\TypeDefine.h"

class IMainServer;
class PbMessageHandle;
class TCPServer;

class ICommunication
{
public:
	ICommunication(IMainServer *pMain = nullptr):m_pMain(pMain){};
	virtual ~ICommunication() { m_pMain = nullptr; };
	virtual PbMessageHandle* GetPbMessageHandle() = 0;
	virtual TCPServer* GetTCPServer() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual void RegisterMessageHandle(unsigned, std::function<void(const unsigned, SDataExchange*)>) = 0;

protected:
	IMainServer *m_pMain;
};
