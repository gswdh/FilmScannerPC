#include <QObject>
#include <QTime>
#include <QThread>
#include <QCoreApplication>

#include "scanner.h"
#include <iostream>
#include <fstream>

class scannerWorker : public QObject
{
    Q_OBJECT

public:
    scannerWorker();
    ~scannerWorker();

public slots:
    void lineWorker();
    void loopStop();

signals:
    void lineDone(uint8_t * pline);

private:
    uint8_t loop_run = 0;
    scanner scan;

};
