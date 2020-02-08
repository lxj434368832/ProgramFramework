#pragma once
#include <string>
#include "IMainServer.h"
#include "../CommonFile/TypeDefine.h"

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	SServerConfig* GetServerConfig() override;
	IModelManage* GetModelManage() override;
	IControllerManage* GetControllerManage() override;
	IMessageHandle* GetMessageHandle() override;
	ICommunication* GetCommunication() override;

	bool StartServer();
	void StopServer();

private:
	bool ReadConfigFile();

private:
	SServerConfig		m_srvConfig;		//服务配置项
	IModelManage		*m_pModel;			//模型管理
	IControllerManage	*m_pController;		//控制器
	IMessageHandle*		m_pMessage;			//消息模块
	ICommunication*		m_pCommunication;	//通讯模块
};

