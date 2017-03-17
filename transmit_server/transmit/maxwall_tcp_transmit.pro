#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T11:12:14
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maxwall_tcp_transmit
TEMPLATE = app


SOURCES += main.cpp\
        tcptransmit.cpp \
    server.cpp \
    tcpclientsocket.cpp \
    util.cpp

HEADERS  += tcptransmit.h \
    server.h \
    tcpclientsocket.h \
    util.h

RESOURCES += \
    icon/icon.qrc
