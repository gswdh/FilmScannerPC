#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void write_to_outputlabel(QString message);
    void draw_in_image_window(void);

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButtonPreview_clicked();

    void on_pushButtonRefreshDevices_clicked();

    void on_pushDirectory_clicked();

    void on_pushStart_clicked();

    void on_pushPreview_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
