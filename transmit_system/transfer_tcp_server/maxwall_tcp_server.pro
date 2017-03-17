#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T15:54:49
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maxwall_tcp_server
TEMPLATE = app


SOURCES += main.cpp\
        tcpserver.cpp \
    server.cpp \
    tcpclientsocket.cpp \
    util.cpp

HEADERS  += tcpserver.h \
    server.h \
    tcpclientsocket.h \
    util.h

RESOURCES += \
    icon/icon.qrc
