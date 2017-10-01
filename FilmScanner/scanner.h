#ifndef SCANNER_H
#define SCANNER_H

#include "ftdi.h"
#include <QObject>

class scanner : public QObject
{



public:
    scanner();

    int StartScan(int gain, int offset, int brightness, long lines);
    int StopScan();

public slots:
    void StartCall();

private:
    int scanner_running = 0;



};

#endif // SCANNER_H
