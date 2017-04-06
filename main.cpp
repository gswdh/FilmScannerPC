#include "mainwindow.h"
#include <QApplication>
#include "ftdi.h"
#include "file.h"


int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    char file_dest[] = "frame1.pgm";

    file myFile = file(file_dest, 1, 2048, BIT_DEPTH_8);

    unsigned short int line[2048];

    for(int i = 0; i < 2048; i++)line[i] = i % 256;

    myFile.write_line(line);

    myFile.close();

    return a.exec();
}
