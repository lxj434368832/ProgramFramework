#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "..\ViewManage\ViewColleague.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog, public ViewColleague
{
    Q_OBJECT

public:
    explicit LoginDialog(ViewMediator *mdt, QWidget *parent = 0);
    ~LoginDialog();

private slots:
	//登陆提示消息
	void slotLoginTipMsg(std::string strMsg);
	//登陆消息通知
	void slotLoginMessageNt(bool bSuccess, std::string strMsg);

    void on_btnLogin_clicked();

    void on_btnCancel_clicked();

private:
	Ui::LoginDialog *ui;
	bool    m_bLoginLock = false;
};

#endif // LOGINDIALOG_H
