#pragma once
#include <QObject>
#include <QThread>
#include <QMap>
#include "IControllerManage.h"

class ControllerManage : public QObject, public IControllerManage
{
    Q_OBJECT
public:
    ControllerManage(IMainClient *_main = nullptr);
    ~ControllerManage() override;
    MainController* GetMainController() override;
    StatisticController* GetStatisticController() override;

	bool Start() override;
	void Stop() override;

signals:

private slots:

private:
    MainController      *m_mainController = nullptr;
    StatisticController *m_statisticController = nullptr;
};

