#pragma once

#include <QMap>
#include <QList>
#include <QVariant>
#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
    ModelManage(IMainClient *_main = nullptr);
    ~ModelManage();

	bool Start() override;
	void Stop() override;

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

