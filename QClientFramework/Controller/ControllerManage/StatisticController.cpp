#include "StatisticController.h"
#include "../../Model/ModelManage/StatisticModel.h"

StatisticController::StatisticController(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(signalImportData(QString)),this, SLOT(slotImportData(QString)));
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
    m_statisticModel = dynamic_cast<StatisticModel*>(m_main->GetModelInterface());
    if(nullptr == m_model)
    {
        loge()<<QStringLiteral("获取模型管理失败!");
        return false;
    }
    return true;
}

void StatisticController::Stop()
{

}

void StatisticController::AddData(QString qstrPeriod, QString qstrNum)
{

}

const QMap<QString, QString> StatisticController::GetNumberList()
{

}

void StatisticController::slotImportData(QString qstrFilePath)
{

}

void StatisticController::slotExecuteStatistic(int iStatisticCount, int iStatisticFigure, QVector<int> rank)
{

}
