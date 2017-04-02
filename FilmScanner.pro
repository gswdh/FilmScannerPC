#-------------------------------------------------
#
# Project created by QtCreator 2017-04-02T17:59:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilmScanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ftd2xx.h

FORMS    += mainwindow.ui

DISTFILES +=


win32: LIBS += -L$$PWD/./ -lftd2xx

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
