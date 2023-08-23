#include "MMainWindow.h"
#include "ui_MMainWindow.h"
#include "..\..\Controller\ControllerManage\include\IControllerManage.h"
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

void MMainWindow::on_tabBusiness_tabBarDoubleClicked(int index)
{
    QWidget *widget = ui->tabBusiness->widget(index);
    widget->setWindowTitle(ui->tabBusiness->tabText(index));
    ui->tabBusiness->removeTab(index);
    widget->setWindowFlags(Qt::Window);
    widget->show();
    widget->raise();
    widget->activateWindow();
    widget->move(50, 50);
}

void MMainWindow::slotBackToTab(QWidget* widget, QString qstrTitle)
{
    widget->setWindowFlags(Qt::Widget);
    ui->tabBusiness->addTab(widget, qstrTitle);
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
