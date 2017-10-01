#include "mainwindow.h"
#include "ui_mainwindow.h"

ftdi ft;
scanner sc;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(startstop_button_clicked()),
                     &sc, SLOT(StartCall()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_refresh_button_clicked()
{
    // Create a new device list
    FTDIDevices list;

    ft.ListDevices(&list);

    std::cout << "Number of devices attached: " << list.ndevs << std::endl;
    std::cout << list.device1 << std::endl;
    std::cout << list.device2 << std::endl;

    ui->devices_box->clear();

    ui->devices_box->addItem(QString(list.device1));
    ui->devices_box->addItem(QString(list.device2));


}

void MainWindow::on_startstop_button_clicked()
{
    emit startstop_button_clicked(
                ui->devices_box->itemData(
                    ui->devices_box->currentIndex()));
}
