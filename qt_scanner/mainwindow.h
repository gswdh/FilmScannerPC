#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include "scannerWorker.h"
#include <iostream>
#include <fstream>

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
    void on_b_start_clicked();
    void on_b_stop_clicked();
    void receiveLine(uint8_t * line);

signals:
    void start_Clicked();
    void stop_Clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
