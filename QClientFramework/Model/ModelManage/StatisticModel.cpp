#include "StatisticModel.h"
#include "../../CommonFile/CommonDefine.h"

StatisticModel::StatisticModel()
{

}

bool StatisticModel::Start()
{
    return true;
}

void StatisticModel::Stop()
{

}

int StatisticModel::SaveNumberList(QList<QList<QVariant> > rows)
{
    if (0 == rows.size())
    {
        loge() << QStringLiteral("保存数据失败，长度为0！");
        return 0;
    }
    rows.pop_front();		//去掉标题栏

    m_mapNumberList.clear();
    for (QList<QVariant> row : rows)
    {
        QString qstrPeriod = row.at(1).toString();
        QString qstrNum = row.at(2).toString();
        m_mapNumberList[qstrPeriod] = qstrNum;
    }
    return m_mapNumberList.count();
}

int StatisticModel::AddNumberData(QString qstrPeriod, QString qstrNum)
{
    if (qstrPeriod.isEmpty() || qstrNum.isEmpty())
    {
        loge()<<QString::fromLocal8Bit("添加号码数据失败! period:%1 number:%2").arg(qstrPeriod).arg(qstrNum);
        return -1;
    }

    int icount = qstrNum.split(',').size();
    if(10 != icount)
    {
        loge()<<QString::fromLocal8Bit("添加的号码有误，个数不为10。number:%1  个数:%2").arg(qstrNum).arg(icount);
        return -1;
    }

    m_mapNumberList[qstrPeriod] = qstrNum;
    return m_mapNumberList.count();
}

QList<QStringList> StatisticModel::ExecuteStatistic(int iStatisticCount, int iStatisticFigure, QVector<int> rank)
{
    QList<QStringList> rows;
    int iCount = m_mapNumberList.size();
    if (0 == iCount || iCount < iStatisticCount)
    {
        loge() << QString::fromLocal8Bit("统计失败，总期数为:%1 统计期数为:%2").arg(iCount).arg(iStatisticCount);
        return rows;
    }
   ::memset(m_aHitCount,0,sizeof(m_aHitCount));

    double dAvg;
   int iRow = 0;
   auto iter = m_mapNumberList.begin();
   for (iter = iter + (iCount - iStatisticCount); iter != m_mapNumberList.end(); iter++)
   {
       QStringList row;
       row.append(iter.key());
       //添加号码
       QString qstrNums = iter.value();
       row.append(qstrNums);

        //添加平均值
       dAvg = (iRow + 1) * iStatisticFigure / 10;
       row.append(QString::number(dAvg, 'f' ,1));

       //添加各个数字的热度
       StatisticOneHeat(qstrNums, rank);
       iCount = rank.size();
       for (int i = 0; i < iCount; i++)
       {
           row.append(QString::number(m_aHitCount[i]));
       }
       rows.append(row);
   }
    return rows;
}

const QMap<QString, QString> StatisticModel::GetNumberList()
{
    return m_mapNumberList;
}

void StatisticModel::StatisticOneHeat(QString &qstrNums, QVector<int> rank)
{
    int idx = 0;
    QStringList numList = qstrNums.split(',');
    for (QString qstrNum : numList)
    {
        int iNum = qstrNum.toInt() - 1;
        if (1 == rank[idx++])
        {
            m_aHitCount[iNum]++;
        }
    }
}
