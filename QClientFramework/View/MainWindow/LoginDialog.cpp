#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "..\..\Controller\ControllerManage\MainController.h"
#include "..\..\Controller\ControllerManage\IControllerManage.h"

LoginDialog::LoginDialog(ViewMediator *mdt, QWidget *parent) :
	QDialog(parent),
	ViewColleague(mdt),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    std::string strUserName = ui->leUserName->text().toLocal8Bit().data();
    std::string strPassword = ui->lePassword->text().toLocal8Bit().data();
	//if (m_pCtrlMng->GetMainController()->UserLogin(strUserName, strPassword))
	//{
	//	accept();
	//}
	//else
	//{
	//	ui->lbMessage->setText(QStringLiteral("ÓÃ»§µÇÂ¼Ê§°Ü£¡"));
	//}
}

void LoginDialog::on_btnCancel_clicked()
{
	reject();
}
