#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T15:44:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = s4wdchart
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \
    tgchart.cpp

HEADERS  += mainwindow.h \
          qcustomplot.h \
    tgchart.h

FORMS    += mainwindow.ui
