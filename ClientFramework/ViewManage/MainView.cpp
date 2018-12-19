#include "MainView.h"



MainView::MainView(IMainClient *_main = nullptr)
	:IViewInterface(_main)
{
}


MainView::~MainView()
{
}

bool MainView::Start()
{
	return false;
}

void MainView::Stop()
{
}
