#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T11:15:59
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maxwall_tcp_client
TEMPLATE = app


SOURCES += main.cpp\
        tcpclient.cpp

HEADERS  += tcpclient.h

RESOURCES += \
    icon/icon.qrc
