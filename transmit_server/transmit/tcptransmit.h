#ifndef TCPTRANSMIT_H
#define TCPTRANSMIT_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QSettings>
#include "server.h"
#include "util.h"

class TcpTransmit : public QDialog
{
    Q_OBJECT

public:
    TcpTransmit(QWidget *parent = 0);
    ~TcpTransmit();
    QSerialPort serial;//串口实例
    void initSeialPort();//初始化串口函数

private:
    QSystemTrayIcon *systemTray;
    QAction *quitAct;
    QMenu *pContextMenu;
    int port;
    Server *server;
    QByteArray content;

signals:
    void sendCmd(QString, int);

private slots:
    void serialRead();
    void serialWrite(QByteArray);
};

#endif // TCPTRANSMIT_H
