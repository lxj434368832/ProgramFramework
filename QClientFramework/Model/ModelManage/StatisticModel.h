#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H

#include <QMap>
#include <QList>
#include <QVariant>

class StatisticModel
{
public:
    StatisticModel();
    bool Start();
    void Stop();

    int SaveNumberList(QList<QList<QVariant>>);
    int AddNumberData(QString qstrPeriod, QString qstrNum);
    QList<QStringList> ExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

    const QMap<QString, QString> GetNumberList();

private:
    void StatisticOneHeat(QString &qstrNums, QVector<int> rank);

private:

    QMap<QString, QString>        m_mapNumberList;     //期数，号码
    int                                                m_aHitCount[10];        //1--10命中的次数
};

#endif // STATISTICMODEL_H
