#include "formAddData.h"
#include "ui_formAddData.h"

formAddData::formAddData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formAddData)
{
    ui->setupUi(this);
}

formAddData::~formAddData()
{
    delete ui;
}

void formAddData::SetLastPeriod(QString qstrPeriod)
{
	if (qstrPeriod.isEmpty()) return;
    quint64 ulPeriod = qstrPeriod.toULongLong() + 1;
    qstrPeriod = QString::number(ulPeriod);
    ui->lePeriod->setText(qstrPeriod);
}

void formAddData::GetData(QString &qstrPeriod, QString &qstrNum)
{
    qstrPeriod = ui->lePeriod->text();
    qstrNum = ui->leLotteryNum->text();
}

void formAddData::on_btnSure_clicked()
{
    accept();
}

void formAddData::on_btnCancel_clicked()
{
    reject();
}
