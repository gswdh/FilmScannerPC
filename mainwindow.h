#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

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

private slots:
    void on_pushButtonStart_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
