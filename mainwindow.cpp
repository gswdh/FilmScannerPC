#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;

    QPixmap picture("D:/FilmScannerPC/background.png");
    ui->labelPicture->setPixmap(picture.scaled(120, 160, Qt::KeepAspectRatio));
}

void MainWindow::on_pushButtonStart_clicked()
{
    QString scan_resolution = ui->lineEditReso->text();
    QString scan_frames = ui->lineEditFrames->text();



    int resolution = scan_resolution.toInt();
    int frames = scan_frames.toInt();

    int seconds = int(resolution) * int(resolution) * 1.5 * int(frames) * 1.1 / 1000000;

    QString out_seconds = QString::number(seconds % 60);
    QString out_minutes = QString::number(seconds / 60);

    QString msg;
    msg += "Scan beginning at ";
    msg += scan_resolution;
    msg += "dpi for ";
    msg += scan_frames;
    msg += " frames. Scan time is ";
    msg += out_minutes;
    msg += "m ";
    msg += out_seconds;
    msg += "s.";

    ui->labelOutput->setText(msg);
}

void MainWindow::write_to_outputlabel(QString message)
{
    ui->labelOutput->setText(message);
}
