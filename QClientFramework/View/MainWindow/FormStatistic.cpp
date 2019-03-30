#include "FormStatistic.h"
#include "ui_FormStatistic.h"
#include "../../Controller/ControllerManage/ControllerManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "FormLotteryNumber.h"
#include <QButtonGroup>
#include <QCloseEvent>
#include <QElapsedTimer>

FormStatistic::FormStatistic(ViewMediator *mdt, QWidget *parent) :
    QWidget(parent),
    ViewColleague (mdt),
    ui(new Ui::FormStatistic)
{
    ui->setupUi(this);

    int index = 0;
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(false);
    btnGroup->addButton(ui->cbrOne, index++);
    btnGroup->addButton(ui->cbrTwo, index++);
    btnGroup->addButton(ui->cbrThree, index++);
    btnGroup->addButton(ui->cbrFour, index++);
    btnGroup->addButton(ui->cbrFive, index++);
    btnGroup->addButton(ui->cbrSix, index++);
    btnGroup->addButton(ui->cbrSeven, index++);
    btnGroup->addButton(ui->cbrEight, index++);
    btnGroup->addButton(ui->cbrNine, index++);
    btnGroup->addButton(ui->cbrTen, index++);
    connect(btnGroup,SIGNAL(buttonToggled(int,bool)),this, SLOT(slotGroupButtonToggled(int,bool)));

	//初始化统计列表窗体
	ui->twStatisticList->horizontalHeader()->setDefaultSectionSize(50);
	ui->twStatisticList->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    QStringList headers;
    headers.append(QString::fromLocal8Bit("期号"));
    headers.append(QString::fromLocal8Bit("开奖号"));
    headers.append("AVG");
    headers.append("1");
    headers.append("2");
    headers.append("3");
    headers.append("4");
    headers.append("5");
    headers.append("6");
    headers.append("7");
    headers.append("8");
    headers.append("9");
    headers.append("10");
	ui->twStatisticList->setColumnCount(headers.count());
	ui->twStatisticList->setColumnWidth(0, 135);
    ui->twStatisticList->setColumnWidth(1, 280);
	ui->twStatisticList->setColumnWidth(2, 50);
    ui->twStatisticList->setHorizontalHeaderLabels(headers);

	connect(m_mainLogic, SIGNAL(signalLotteryListChanged(QMap<QString, QString>)),
		this, SLOT(slotLotteryListChanged(QMap<QString, QString>)));

    m_iStatisticCount = ui->sbTatisticPeriod->value();
    m_vctStatisticRank = {0,0,1,1,1,1,1,1,0,0};
	m_iStatisticFigure = 6;
    ExecuteStatistic(m_mainLogic->GetLotteryList());
}

FormStatistic::~FormStatistic()
{
    delete ui;
}

void FormStatistic::closeEvent(QCloseEvent *event)
{
    emit signalBackToTab(this, windowTitle());
    event->ignore();
}

void FormStatistic::on_sbTatisticPeriod_editingFinished()
{
    m_iStatisticCount = ui->sbTatisticPeriod->value();
    ExecuteStatistic(m_mainLogic->GetLotteryList());
}

void FormStatistic::on_cbStatisticMode_currentIndexChanged(int index)
{
	index;
    ExecuteStatistic(m_mainLogic->GetLotteryList());
}

void FormStatistic::slotGroupButtonToggled(int id, bool checked)
{
	if (checked)
	{
		m_vctStatisticRank[id] = 1;
		m_iStatisticFigure++;
	}
	else
	{
		m_vctStatisticRank[id] = 0;
		m_iStatisticFigure--;
	}

    ExecuteStatistic(m_mainLogic->GetLotteryList());
}

void FormStatistic::slotLotteryListChanged(QMap<QString,QString> mapLotteryList)
{
	QElapsedTimer time;
	time.start();
    ExecuteStatistic(mapLotteryList);
	logm() << QStringLiteral("统计总共用时%1毫秒").arg(time.elapsed());
}

void FormStatistic::ExecuteStatistic(const QMap<QString, QString> mapLotteryList)
{
    if(0 == ui->cbStatisticMode->currentIndex())
        StatisticHeat(mapLotteryList);
    else
        StatisticMissing(mapLotteryList);
}

void FormStatistic::StatisticHeat(const QMap<QString, QString> mapLotteryList)
{
    double dAvg;
    int iCount = mapLotteryList.size();
	if (0 == iCount || iCount < m_iStatisticCount)
	{
		loge() << QString::fromLocal8Bit("统计失败，总期数为:%1 统计期数为:%2").arg(iCount).arg(m_iStatisticCount);
		return;
	}

	ui->twStatisticList->setRowCount(m_iStatisticCount);
    ui->leTotalPeriod->setText(QString::number(iCount));
   ::memset(m_aHitCount,0,sizeof(m_aHitCount));
   QTableWidgetItem *item;
   int iRow = 0;
   auto iter = mapLotteryList.begin();
   for (iter = iter + (iCount - m_iStatisticCount); iter != mapLotteryList.end(); iter++)
   {
       int iCol = 0;
       //添加期号
	   item = new QTableWidgetItem(iter.key());
	   item->setTextAlignment(Qt::AlignCenter);
       ui->twStatisticList->setItem(iRow, iCol++, item);

       //添加开奖号
	   QString qstrNums = iter.value();
       ui->twStatisticList->setItem(iRow,  iCol, new QTableWidgetItem);
        FormLotteryNumber   *form = new FormLotteryNumber();
		form->SetLotteryNumbers(qstrNums);
        ui->twStatisticList->setCellWidget(iRow, iCol++, form);

        //添加平均值
       dAvg = (iRow + 1) * 6.0 / 10;
       item = new QTableWidgetItem(QString::number(dAvg, 'f' ,1));
	   item->setTextAlignment(Qt::AlignCenter);
       ui->twStatisticList->setItem(iRow,  iCol++,  item);

       //添加各个数字的热度
       StatisticOneHeat(qstrNums);
	   for (int i = 0; i < 10; i++)
	   {
		   item = new QTableWidgetItem(QString::number(m_aHitCount[i]));
		   item->setTextAlignment(Qt::AlignCenter);
		   ui->twStatisticList->setItem(iRow, iCol++, item);
	   }

	   iRow++;
   }

   HighlightFifthRow();
}

void FormStatistic::StatisticOneHeat(QString& qstrNums)
{
	int idx = 0;
	QStringList numList = qstrNums.split(',');
	for (QString qstrNum : numList)
	{
		int iNum = qstrNum.toInt() - 1;
		if (1 == m_vctStatisticRank[idx++])
		{
			m_aHitCount[iNum]++;
		}
	}
}

void FormStatistic::HighlightFifthRow()
{
	QTableWidgetItem *item = nullptr;
	int iRowCount = ui->twStatisticList->rowCount();
	int iColCount = ui->twStatisticList->colorCount();
	for (int iRow = 4; iRow < iRowCount; iRow+= 5)
	{
		for (int iCol = 0; iCol < iColCount; iCol++)
		{
			item = ui->twStatisticList->item(iRow, iCol);
			if(item)
				item->setBackground(QColor("#deffde"));//#fda4a4
		}
	}
}

void FormStatistic::StatisticMissing(const QMap<QString, QString> mapLotteryList)
{

}
