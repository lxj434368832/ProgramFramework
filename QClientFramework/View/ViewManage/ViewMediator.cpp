#include "ViewMediator.h"

ViewMediator::ViewMediator(IMainClient *pMain)
    : QObject()
{
	m_pMain = pMain;
}

ViewMediator::~ViewMediator()
{
    m_pMain = nullptr;
}
