#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T20:45:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HeartBeat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myserver.cpp

HEADERS  += mainwindow.h \
    myserver.h

FORMS    += mainwindow.ui
