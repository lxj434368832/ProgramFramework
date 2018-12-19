#pragma once
#include "IViewInterface.h"

class MainView : public IViewInterface
{
public:
	MainView(IMainClient *_main = nullptr);
	~MainView();

	bool Start() override;
	void Stop() override;

};

