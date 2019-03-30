#include "ViewMediator.h"

ViewMediator::ViewMediator(IMainClient *mainClient)
    : QObject()
{
    m_mainClient = mainClient;
}

ViewMediator::~ViewMediator()
{
    m_mainClient = nullptr;
}
