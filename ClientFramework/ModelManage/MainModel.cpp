#include "MainModel.h"



MainModel::MainModel(IMainClient *_main)
	:IModelInterface(_main)
{
}


MainModel::~MainModel()
{
}

bool MainModel::Start()
{
	return true;
}

void MainModel::Stop()
{
}
