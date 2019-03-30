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

	bool Start() override;
	void Stop() override;

//外部调用的信号
signals:
    /*************************************************************************
    * function:从本地文件中导入数据
    * param qstrFilePath: 文件路径
    * return:
    *************************************************************************/
    void signalImportData(QString qstrFilePath);

//外部调用的函数
public:
    /*************************************************************************
    * function:增加一行数据到列表中
    * param qstrPeriod:期数
    * param qstrNum:   开奖号
    * return:
    *************************************************************************/
    void AddData(QString qstrPeriod, QString qstrNum);

    const QMap<QString, QString> GetLotteryList();

//通知的信号函数
signals:
    void signalLotteryListChanged(QMap<QString, QString>);

private slots:
    void slotImportData(QString qstrFilePath);

private:
    QThread                                     m_thread;                   //异步线程
    ModelManage                           *m_model;                  //模型管理
};

