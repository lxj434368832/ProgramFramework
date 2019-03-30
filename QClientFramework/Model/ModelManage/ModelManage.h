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

    int SaveLotteryNumberList(QList<QList<QVariant>>);
    void AddLotteryNumberData(QString qstrPeriod, QString qstrNum);

    const QMap<QString, QString> GetLotteryList();

private:

    QMap<QString, QString>        m_mapLotteryList;     //ÆÚÊý£¬¿ª½±ºÅ
};

