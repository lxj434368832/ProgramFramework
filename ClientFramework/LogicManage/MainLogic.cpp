#include "MainLogic.h"



MainLogic::MainLogic(IMainClient *_main = nullptr)
	:ILogicInterface(_main)
{
}


MainLogic::~MainLogic()
{
}

bool MainLogic::Start()
{
	return false;
}

void MainLogic::Stop()
{
}
