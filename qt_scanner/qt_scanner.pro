#-------------------------------------------------
#
# Project created by QtCreator 2018-05-27T13:48:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_scanner
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
        C:\Users\George\Documents\FilmScannerPC\FTDIWrapper\inc \
        C:\Users\George\Documents\FilmScannerPC \
        C:\Users\George\Documents\FilmScannerPC\GSBus\software

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        C:\Users\George\Documents\FilmScannerPC\FTDIWrapper\src\ftdi.cpp \
        C:\Users\George\Documents\FilmScannerPC\GSBus\software\gsbus.cpp \
        C:\Users\George\Documents\FilmScannerPC\scanner.cpp


HEADERS += \
        mainwindow.h\

FORMS += \
        mainwindow.ui

LIBS += \
        C:\Users\George\Documents\FilmScannerPC\FTDIWrapper\windows\ftd2xx.lib

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O0
