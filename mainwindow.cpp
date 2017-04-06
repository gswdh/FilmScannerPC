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
}

void MainWindow::on_pushStart_clicked()
{
    QString scan_resolution = ui->lineEditFrames->text();
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

    ui->pushStart->setText("Stop");
}

void MainWindow::write_to_outputlabel(QString message)
{
    ui->labelOutput->setText(message);
}

void MainWindow::draw_in_image_window()
{
    QImage image(640, 480, QImage::Format_RGB32);

    QRgb value = qRgb(180, 180, 170);

    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
            image.setPixel(x, y, value);
        }
    }

    ui->labelPicOut->setPixmap(QPixmap::fromImage(image));

    ui->labelPicOut->show();
}

void MainWindow::on_pushPreview_clicked()
{
    draw_in_image_window();
}

void MainWindow::on_pushButtonRefreshDevices_clicked()
{

    ui->dropDevice->clear();

    ui->dropDevice->addItem("FS101");

    //ui->dropDevice->itemData(dropDevice->currentIndex());
}

void MainWindow::on_pushDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->labelOutput->setText(dir);
}
