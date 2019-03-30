#include "ViewManage.h"
#include "ViewMediator.h"
#include "../MainWindow/MMainWindow.h"



ViewManage::ViewManage(IMainClient *_main)
	:IViewManage(_main)
{
	m_viewMdt = new ViewMediator(m_main);
}

ViewManage::~ViewManage()
{
    delete m_mainWnd;
    m_mainWnd = nullptr;

    delete m_viewMdt;
    m_viewMdt = nullptr;
}

bool ViewManage::Start()
{
	m_mainWnd = new MMainWindow(m_viewMdt);
    m_mainWnd->show();
    m_mainWnd->raise();
    m_mainWnd->activateWindow();
	return true;
}

void ViewManage::Stop()
{
}
