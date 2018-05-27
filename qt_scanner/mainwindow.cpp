#include "mainwindow.h"
#include "ui_mainwindow.h"

scanner scan;

// Scanner callback, lines will be received here
void scannerCallBack(std::vector<unsigned char>* data)
{
    std::cout << "Line recieved" << std::endl;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup and start the scanner
    scan.init("GS1FPQH5");

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b_start_clicked()
{
    scan.scanStart(scannerCallBack);
}

void MainWindow::on_b_stop_clicked()
{
    scan.scanStop();
}
