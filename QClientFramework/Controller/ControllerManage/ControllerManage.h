#pragma once
/************************************************************************
* filename:ControllerManage.h
* function: 主控制器，用于管理各个控制器
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

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

