#pragma once
#include "ILogicInterface.h"

class MainLogic : public ILogicInterface
{
public:
	MainLogic(IMainClient *_main = nullptr);
	~MainLogic();

	bool Start() override;
	void Stop() override;

};

