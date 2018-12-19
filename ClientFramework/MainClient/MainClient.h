#pragma once

#include "../include/TypeDefine.h"
#include "IMainClient.h"

class MainClient : public IMainClient
{
public:
	MainClient();
	~MainClient();
	virtual ClientConfig* GetClientConfig() override;
	virtual IViewInterface* GetViewInterface() override;
	virtual ILogicInterface* GetLogicInterface() override;
	virtual IModelInterface* GetModelInterface() override;

	virtual ICommunication* GetMainCommunication() override;
	virtual IMessage*	GetMessageModule() override;

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig		m_clConfig;	//øÕªß∂À≈‰÷√œÓ
	IViewInterface*		m_pView;
	ILogicInterface*	m_pLogic;
	IModelInterface*	m_pModel;

	ICommunication*		m_pCommunication;
	IMessage*			m_pMessage;
};

