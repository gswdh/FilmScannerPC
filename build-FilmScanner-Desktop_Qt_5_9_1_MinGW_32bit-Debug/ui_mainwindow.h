/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *refresh_button;
    QComboBox *devices_box;
    QLabel *label;
    QLineEdit *gainlineEdit;
    QSlider *gainSlider;
    QLabel *label_2;
    QLineEdit *offsetlineEdit;
    QSlider *offsetSlider;
    QLabel *label_3;
    QLineEdit *frames_lineedit;
    QSpacerItem *verticalSpacer;
    QPushButton *startstop_button;
    QLabel *preview_label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1024, 720);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(840, 20, 160, 361));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        refresh_button = new QPushButton(verticalLayoutWidget);
        refresh_button->setObjectName(QStringLiteral("refresh_button"));

        verticalLayout->addWidget(refresh_button);

        devices_box = new QComboBox(verticalLayoutWidget);
        devices_box->setObjectName(QStringLiteral("devices_box"));

        verticalLayout->addWidget(devices_box);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        gainlineEdit = new QLineEdit(verticalLayoutWidget);
        gainlineEdit->setObjectName(QStringLiteral("gainlineEdit"));

        verticalLayout->addWidget(gainlineEdit);

        gainSlider = new QSlider(verticalLayoutWidget);
        gainSlider->setObjectName(QStringLiteral("gainSlider"));
        gainSlider->setMaximum(65535);
        gainSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(gainSlider);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        offsetlineEdit = new QLineEdit(verticalLayoutWidget);
        offsetlineEdit->setObjectName(QStringLiteral("offsetlineEdit"));

        verticalLayout->addWidget(offsetlineEdit);

        offsetSlider = new QSlider(verticalLayoutWidget);
        offsetSlider->setObjectName(QStringLiteral("offsetSlider"));
        offsetSlider->setMaximum(65535);
        offsetSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(offsetSlider);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        frames_lineedit = new QLineEdit(verticalLayoutWidget);
        frames_lineedit->setObjectName(QStringLiteral("frames_lineedit"));

        verticalLayout->addWidget(frames_lineedit);

        verticalSpacer = new QSpacerItem(20, 300, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        startstop_button = new QPushButton(verticalLayoutWidget);
        startstop_button->setObjectName(QStringLiteral("startstop_button"));

        verticalLayout->addWidget(startstop_button);

        preview_label = new QLabel(centralWidget);
        preview_label->setObjectName(QStringLiteral("preview_label"));
        preview_label->setGeometry(QRect(40, 20, 768, 512));
        preview_label->setMinimumSize(QSize(768, 512));
        preview_label->setMaximumSize(QSize(768, 512));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        refresh_button->setText(QApplication::translate("MainWindow", "Refresh", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Gain", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Offset", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Frames", Q_NULLPTR));
        startstop_button->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        preview_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
