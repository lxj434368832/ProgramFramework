#pragma once
#include <string>
#include "IMainServer.h"
#include "../CommonFile/TypeDefine.h"

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	ServerConfig* GetServerConfig() override;
	IModelManage* GetModelManage() override;
	IControllerManage* GetControllerManage() override;
	IMessageHandle* GetMessageBusiness() override;
	ITCPCommunication* GetCommunication() override;

	bool Start();
	void Stop();

private:
	bool ReadConfigFile();

private:
	ServerConfig		m_srvConfig;		//服务配置项
	IModelManage		*m_pModel;			//模型管理
	IControllerManage	*m_pController;		//控制器
	IMessageHandle*		m_pMessage;			//消息模块
	ITCPCommunication*	m_pCommunication;	//通讯模块
};

