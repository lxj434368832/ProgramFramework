#pragma once

#include "../CommonFile/TypeDefine.h"
#include "IMainClient.h"

class MainClient : public IMainClient
{
public:
	MainClient();
    ~MainClient() override;
	virtual ClientConfig* GetClientConfig() override;
	virtual IViewManage* GetViewInterface() override;
	virtual IControllerManage* GetControllerInterface() override;
	virtual IModelManage* GetModelInterface() override;

    virtual ITCPCommunication* GetTCPCommunication() override;
	virtual IMessage*	GetMessageModule() override;

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig		m_clConfig;	//øÕªß∂À≈‰÷√œÓ
	IViewManage*		m_pView;
    IControllerManage*	m_pController;
	IModelManage*	m_pModel;

    ITCPCommunication*		m_pTCPCommunication;
    IMessage*                          m_pMessage;
};

