#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include "scanner.h"
#include <vector>
#include <iostream>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
