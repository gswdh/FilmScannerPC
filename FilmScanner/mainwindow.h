#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "iostream"

#include "scanner.h"
#include "ftdi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_refresh_button_clicked();

    void on_startstop_button_clicked();


signals:
    void startstop_button_clicked(QString device);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
