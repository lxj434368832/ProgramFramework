#include "ModelManage.h"
#include "../../CommonFile/CommonDefine.h"



ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
}


ModelManage::~ModelManage()
{
}

bool ModelManage::Start()
{
	return true;
}

void ModelManage::Stop()
{
}

int ModelManage::SaveLotteryNumberList(QList<QList<QVariant> > rows)
{
    if (0 == rows.size())
    {
        loge() << QStringLiteral("保存数据失败，长度为0！");
        return 0;
    }
    rows.pop_front();		//去掉标题栏

    m_mapLotteryList.clear();
    for (QList<QVariant> row : rows)
    {
        QString qstrPeriod = row.at(1).toString();
        QString qstrNum = row.at(2).toString();
        m_mapLotteryList[qstrPeriod] = qstrNum;
    }
    return m_mapLotteryList.size();
}

void ModelManage::AddLotteryNumberData(QString qstrPeriod, QString qstrNum)
{
    if (qstrPeriod.isEmpty() || qstrNum.isEmpty())
    {
        loge()<<QString::fromLocal8Bit("添加开奖号码数据失败! period:%1 number:%2").arg(qstrPeriod).arg(qstrNum);
        return;
    }

    int icount = qstrNum.split(',').size();
    if(10 != icount)
    {
        loge()<<QString::fromLocal8Bit("添加的开奖号码有误，个数不为10。number:%1  个数:%2").arg(qstrNum).arg(icount);
        return;
    }

    m_mapLotteryList[qstrPeriod] = qstrNum;
}

const QMap<QString, QString> ModelManage::GetLotteryList()
{
    return m_mapLotteryList;
}
