#include "ControllerManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "../../Model/ModelManage/ModelManage.h"
#include "../../3rdParty/ReadWriteExcel/include/ReadWriteExcel.h"
#include <QElapsedTimer>


ControllerManage::ControllerManage(IMainClient *_main) :
	QObject(),
    IControllerManage(_main)
{

}

ControllerManage::~ControllerManage()
{
}

MainController *ControllerManage::GetMainController()
{

}

StatisticController *ControllerManage::GetStatisticController()
{

}

bool ControllerManage::Start()
{

}

void ControllerManage::Stop()
{
    m_model = nullptr;
}

void ControllerManage::slotImportData(QString qstrFilePath)
{
    ReadWriteExcel rdExcel;
    if(nullptr == m_model)  return;
    rdExcel.OpenExcel(qstrFilePath);
   int iCount = m_model->SaveNumberList(rdExcel.ReadAllData());

   emit signalNumberListChanged(iCount);
}

void ControllerManage::slotExecuteStatistic(int iStatisticCount, int iStatisticFigure, QVector<int> rank)
{
    emit signalStatisticResultNotify( m_model->ExecuteStatistic(iStatisticCount, iStatisticFigure, rank));
}

void ControllerManage::AddData(QString qstrPeriod, QString qstrNum)
{
     if(nullptr == m_model)  return;
    int iCount = m_model->AddNumberData(qstrPeriod, qstrNum);
    emit signalNumberListChanged(iCount);
}

const QMap<QString, QString> ControllerManage::GetNumberList()
{
     if(nullptr == m_model)
     {
         QMap<QString, QString> map;
         return map;
     }
    return m_model->GetNumberList();
}
