#include "StatisticController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/StatisticModel.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../3rdParty/ReadWriteExcel/include/ReadWriteExcel.h"
#include <QElapsedTimer>

StatisticController::StatisticController(IMainClient *main)
    : ControllerColleague(main)
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QList<QStringList>>("QList<QStringList>");

    connect(this, SIGNAL(signalImportData(QString)),this, SLOT(slotImportData(QString)));
    connect(this, SIGNAL(signalExecuteStatistic(int,int,QVector<int>)), this, SLOT(slotExecuteStatistic(int,int,QVector<int>)));
    moveToThread(&m_thread);
    m_thread.start();
}

StatisticController::~StatisticController()
{
    m_thread.quit();
    m_thread.wait();
    //    m_thread.terminate();
}

bool StatisticController::Start()
{
    m_statisticModel = m_model->GetStatisticModel();
    if(nullptr == m_statisticModel)
    {
        loge()<<QStringLiteral("获取统计模型失败!");
        return false;
    }
    return true;
}

void StatisticController::Stop()
{
    m_statisticModel = nullptr;
}

void StatisticController::AddData(QString qstrPeriod, QString qstrNum)
{
    if(nullptr == m_statisticModel)  return;
   int iCount = m_statisticModel->AddNumberData(qstrPeriod, qstrNum);
   emit signalNumberListChanged(iCount);
}

void StatisticController::slotImportData(QString qstrFilePath)
{
    ReadWriteExcel rdExcel;
    if(nullptr == m_statisticModel)  return;
    rdExcel.OpenExcel(qstrFilePath);
   int iCount = m_statisticModel->SaveNumberList(rdExcel.ReadAllData());

   emit signalNumberListChanged(iCount);
}

void StatisticController::slotExecuteStatistic(int iStatisticCount, int iStatisticFigure, QVector<int> rank)
{
    if(nullptr == m_statisticModel)  return;
    emit signalStatisticResultNotify( m_statisticModel->ExecuteStatistic(iStatisticCount, iStatisticFigure, rank));
}

const QMap<QString, QString> StatisticController::GetNumberList()
{
    if(nullptr == m_model)
    {
        QMap<QString, QString> map;
        return map;
    }
   return m_statisticModel->GetNumberList();
}
