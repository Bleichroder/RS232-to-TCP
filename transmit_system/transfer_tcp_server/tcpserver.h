#ifndef TCPSERVER_H
#define TCPSERVER_H
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

class TcpServer : public QDialog
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = 0);
    ~TcpServer();
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

#endif // TCPSERVER_H
