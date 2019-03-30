#ifndef MEDIATORWIDGET_H
#define MEDIATORWIDGET_H

#include <QObject>

class IMainClient;

class ViewMediator : public QObject
{
    Q_OBJECT
public:
    explicit ViewMediator(IMainClient *mainClient);
    ~ViewMediator();

    inline IMainClient* GetMainClient(){return m_mainClient;}

signals:

public slots:

private:
    IMainClient *m_mainClient;

};

#endif // MEDIATORWIDGET_H
