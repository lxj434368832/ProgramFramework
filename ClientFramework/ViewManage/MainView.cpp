#include "MainView.h"



MainView::MainView(IMainClient *_main)
	:IViewInterface(_main)
{
}


MainView::~MainView()
{
}

bool MainView::Start()
{
	return true;
}

void MainView::Stop()
{
}
