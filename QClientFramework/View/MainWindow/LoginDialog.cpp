#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "..\..\Controller\ControllerManage\include\IMainController.h"
#include "..\..\Controller\ControllerManage\include\IControllerManage.h"
#include "..\..\3rdParty\MConfigManage\include\MConfigManage.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\..\CommonFile\CommonDefine.h"

LoginDialog::LoginDialog(ViewMediator *mdt, QWidget *parent) :
	QDialog(parent),
	ViewColleague(mdt),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	MConfigManage *pCfgMng = m_pMain->GetConfigManage();
	ui->leUserName->setText(pCfgMng->userValue("Client", "UserName", "").toString());
	ui->lePassword->setText(pCfgMng->userValue("Client", "Password", "").toString());

	connect(MainCtrl, SIGNAL(signalLoginTipMsg(std::string)), SLOT(slotLoginTipMsg(std::string)));
	connect(MainCtrl, SIGNAL(signalLoginMessageNt(bool, std::string)),
		SLOT(slotLoginMessageNt(bool, std::string)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slotLoginTipMsg(std::string strMsg)
{
	QString qstrMsg = QString("<font color='black'>%1</font>").arg(QString::fromLocal8Bit(strMsg.c_str()));
	ui->lbMessage->setText(qstrMsg);
	logm() << strMsg;
	
}

void LoginDialog::slotLoginMessageNt(bool bSuccess, std::string strMsg)
{
	m_bLoginLock = false;
	if (bSuccess)
	{
		accept();
		logm() << strMsg;
	}
	else
	{
		ui->lbMessage->setText(QString::fromLocal8Bit(strMsg.data()));
		logw() << strMsg;
	}
}

void LoginDialog::on_btnLogin_clicked()
{
	if (m_bLoginLock) return;
	std::string strUserName = ui->leUserName->text().toLocal8Bit().data();
	std::string strPassword = ui->lePassword->text().toLocal8Bit().data();

	MConfigManage *pCfgMng = m_pMain->GetConfigManage();
	pCfgMng->setUserValue("Client", "UserName", strUserName.data());
	pCfgMng->setUserValue("Client", "Password", strPassword.data());

	emit MainCtrl->signalUserLogin(strUserName, strPassword);
	m_bLoginLock = true;
}

void LoginDialog::on_btnCancel_clicked()
{
	reject();
}
