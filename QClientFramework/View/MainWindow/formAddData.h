#ifndef FORMADDDATA_H
#define FORMADDDATA_H

#include <QDialog>

namespace Ui {
class formAddData;
}

class formAddData : public QDialog
{
    Q_OBJECT

public:
    explicit formAddData(QWidget *parent = nullptr);
    ~formAddData();
    void SetLastPeriod(QString qstrPeriod);

    void GetData(QString &qstrPeriod, QString &qstrNum);

private slots:
    void on_btnSure_clicked();

    void on_btnCancel_clicked();

private:
    Ui::formAddData *ui;
};

#endif // FORMADDDATA_H
