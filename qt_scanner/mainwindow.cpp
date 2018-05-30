#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup and start the scanner
    //scan.init("GS1FPQH5");

    QThread* scannerThread = new QThread;
    scannerWorker* worker = new scannerWorker();

    worker->moveToThread(scannerThread);

    connect(scannerThread, SIGNAL(started()), worker, SLOT(lineWorker()));
    connect(this, SIGNAL(stop_Clicked()), worker, SLOT(loopStop()));

    scannerThread->start();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b_start_clicked()
{
    //scan.scanStart(scannerCallBack);

}

void MainWindow::on_b_stop_clicked()
{
    //scan.scanStop();
    emit stop_Clicked();
}
