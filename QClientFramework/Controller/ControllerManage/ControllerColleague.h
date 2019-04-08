#pragma once

#include <QObject>

class IMainClient;
class IControllerManage;
class IModelManage;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(IMainClient*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_main = nullptr;
    IControllerManage *m_controller = nullptr;
    IModelManage        *m_model = nullptr;
};
