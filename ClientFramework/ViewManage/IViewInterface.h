#pragma once

class IMainClient;

class IViewInterface
{
public:
	IViewInterface(IMainClient *_main = nullptr) :m_main(_main) {}
	virtual ~IViewInterface() { m_main = nullptr; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

	IMainClient *m_main = nullptr;
};
