#pragma once

#include "TypeDefine.h"
#include "IMainClient.h"

class IOCPClient;
class INetInterface;

class MainClient : public IMainClient
{
public:
	MainClient();
	~MainClient();

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig	m_srvConfig;	//服务配置项
	INetInterface	*m_pClientList;	//客户端列表
	IOCPClient		*m_pIOCPClient;	//IOCP 客户端
};

