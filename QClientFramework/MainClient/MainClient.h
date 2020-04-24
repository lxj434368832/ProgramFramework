#pragma once

#include "../CommonFile/TypeDefine.h"
#include "IMainClient.h"

class MainClient : public IMainClient
{
public:
	MainClient();
    ~MainClient();
	ClientConfig* GetClientConfig() override;
	MConfigManage* GetConfigManage() override;
	IViewManage* GetViewManage() override;
	IControllerManage* GetControllerManage() override;
	IModelManage* GetModelManage() override;

	bool StartClient() override;
	void StopClient() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig		m_clConfig;	//客户端配置项
	MConfigManage*		m_pCfgMng;	//配置管理
	IViewManage*		m_pView;
    IControllerManage*	m_pController;
	IModelManage*		m_pModel;
};

