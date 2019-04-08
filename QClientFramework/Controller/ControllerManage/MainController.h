#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);

    bool Start();
    void Stop();

signals:

public slots:

};

#endif // MAINCONTROLLER_H
