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
    void on_btnLogin_clicked();

    void on_btnCancel_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
