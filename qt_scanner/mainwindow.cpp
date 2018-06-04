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
    connect(worker, SIGNAL(lineDone(uint8_t*)), this, SLOT(receiveLine(uint8_t*)));

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
    std::cout << "Start clicked" << std::endl;

}

void MainWindow::on_b_stop_clicked()
{
    //scan.scanStop();
    std::cout << "Stop clicked" << std::endl;
    emit stop_Clicked();
}

void MainWindow::receiveLine(uint8_t * line)
{
    std::cout << "Line received" << std::endl;
}
