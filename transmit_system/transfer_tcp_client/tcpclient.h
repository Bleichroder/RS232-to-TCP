#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QTextCodec>
#include <QTimer>

class TcpClient : public QDialog
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = 0);
    ~TcpClient();
    QSerialPort serial;//串口实例
    void initSeialPort();//初始化串口函数
    void receive(QString, int);
    QFile *file;

private:
    QSystemTrayIcon *systemTray;
    QAction *quitAct, *connAct, *disconnAct;
    QMenu *pContextMenu;
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpSocket;
    QByteArray content;
    bool hasconnected;
    QTimer *timer;

public slots:
    void slotConnected();
    void slotDisconnected();
    void connClick();
    void disconnClick();
    void dataReceived();
    void serialRead();
    void timeout();
};

#endif // TCPCLIENT_H
