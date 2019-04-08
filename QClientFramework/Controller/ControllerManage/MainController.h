#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QThread>
#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(IMainClient*);
    ~MainController() override;

    bool Start();
    void Stop();

signals:

public slots:

private:
    MainModel   *m_mainModel;
    QThread       m_thread;                   //“Ï≤Ωœﬂ≥Ã
};

#endif // MAINCONTROLLER_H
