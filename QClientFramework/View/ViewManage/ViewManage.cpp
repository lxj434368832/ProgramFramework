#include "ViewManage.h"
#include "ViewMediator.h"
#include "../MainWindow/MMainWindow.h"
#include "../../CommonFile/CommonDefine.h"



ViewManage::ViewManage(IMainClient *_main)
	:IViewManage(_main)
{
	m_viewMdt = new ViewMediator(m_main);
	m_mainWnd = nullptr;
}

ViewManage::~ViewManage()
{
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
	RELEASE(m_mainWnd);
}
