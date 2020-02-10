#pragma once

#include "ICommunication.h"
#include "../../CommonFile/TypeDefine.h"

class Communication : public ICommunication
{
public:
    Communication(IMainClient *main = nullptr);
    ~Communication();
	ServerConnect* GetServerConnect() override;

	bool Initialize() override;
	void Uninitialize() override;

	bool ConnectServer() override;

private:
	ServerConnect*		m_pConnect;

};

