#include <QObject>
#include <QTime>
#include <QThread>
#include <QCoreApplication>

#include <iostream>

class scannerWorker : public QObject
{
    Q_OBJECT

public:
    scannerWorker();
    ~scannerWorker();

public slots:
    void lineWorker();
    void loopStop();

private:
    uint8_t loop_run = 0;

};
