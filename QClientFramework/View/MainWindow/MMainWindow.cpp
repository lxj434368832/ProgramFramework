#include "MMainWindow.h"
#include "ui_MMainWindow.h"
#include "FormStatistic.h"
#include "..\..\Controller\ControllerManage\ControllerManage.h"
#include "..\..\CommonFile\CommonDefine.h"
#include <QFileDialog>
#include <QElapsedTimer>

MMainWindow::MMainWindow(ViewMediator *mdt, QWidget *parent) :
    MFramelessWidget(parent),
    ViewColleague (mdt),
    ui(new Ui::MMainWindow)
{
    ui->setupUi(this);
    ui->frameStatusBar->setVisible(false);
    on_btnAddStatistic_clicked();
}

MMainWindow::~MMainWindow()
{
    delete ui;
}

void MMainWindow::on_btnClose_clicked()
{
    close();
}

void MMainWindow::on_btnImportData_clicked()
{
    QString qstrFilePath = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("请选择要导入的Excel文件"),
		"", QString::fromLocal8Bit("Excel 工作薄(*.xlsx *.xls)"));

	if (qstrFilePath.isEmpty()) return;
	if (false == QFile::exists(qstrFilePath)) return;
    emit m_controller->signalImportData(QDir::toNativeSeparators(qstrFilePath));
}

void MMainWindow::on_btnAddData_clicked()
{
	QString qstrPeriod;
    QString qstrNum;
    const QMap<QString, QString> lotteryList = m_controller->GetNumberList();
	if (false == lotteryList.isEmpty())
	{
		qstrPeriod = lotteryList.lastKey();
    }

    m_controller->AddData(qstrPeriod, qstrNum);
}

void MMainWindow::on_btnAddStatistic_clicked()
{
    int iItemCount = ui->tabStatistic->count();
    FormStatistic *form = new FormStatistic(m_mediator);
    connect(form, SIGNAL(signalBackToTab(QWidget*, QString)), this, SLOT(slotBackToTab(QWidget*, QString)));

    ui->tabStatistic->addTab(form, QString::fromLocal8Bit("统计项%1").arg(iItemCount + 1));
    ui->tabStatistic->setCurrentIndex(iItemCount);
}

void MMainWindow::on_tabStatistic_tabBarDoubleClicked(int index)
{
    QWidget *widget = ui->tabStatistic->widget(index);
    widget->setWindowTitle(ui->tabStatistic->tabText(index));
    ui->tabStatistic->removeTab(index);
    widget->setWindowFlags(Qt::Window);
    widget->show();
    widget->raise();
    widget->activateWindow();
    widget->move(50, 50);
}

void MMainWindow::slotBackToTab(QWidget* widget, QString qstrTitle)
{
    widget->setWindowFlags(Qt::Widget);
    ui->tabStatistic->addTab(widget, qstrTitle);
}

void MMainWindow::on_btnMinimum_clicked()
{
    showMinimized();
}

void MMainWindow::on_btnMaximum_clicked()
{
    if(isMaximized())
        showNormal();
    else
        showMaximized();
}
