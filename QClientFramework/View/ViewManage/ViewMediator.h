#ifndef MEDIATORWIDGET_H
#define MEDIATORWIDGET_H

#include <QObject>

class IMainClient;

class ViewMediator : public QObject
{
    Q_OBJECT
public:
	explicit ViewMediator(IMainClient *pMain);
    ~ViewMediator();

    inline IMainClient* GetMainClient(){return m_pMain;}

signals:

public slots:

private:
    IMainClient *m_pMain;

};

#endif // MEDIATORWIDGET_H
