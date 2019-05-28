#include "MMainWindow.h"
#include "ui_MMainWindow.h"
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
}

MMainWindow::~MMainWindow()
{
    delete ui;
}

void MMainWindow::on_btnClose_clicked()
{
    close();
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
