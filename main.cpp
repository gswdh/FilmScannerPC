#include "mainwindow.h"
#include <QApplication>
#include "ftd2xx.h"

int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    DWORD numDevs;
    FT_STATUS ftStatus;

    // Open device number 1
    ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);

    QString n_devices = QString::number(numDevs);

    QString msg;
    msg += "Number of devices: ";
    msg += n_devices;

    w.write_to_outputlabel(msg);

    return a.exec();
}
