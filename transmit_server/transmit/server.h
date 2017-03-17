#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include <QApplication>
#include "tcpclientsocket.h"
#include "util.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0, int port = 0);
    QList<TcpClientSocket*> tcpClientSocketList;
signals:
    void sendserial(QByteArray);
public slots:
    void sendCmd(QString, int);
    void slotDisconnected(int);
    void slotsendserial(QByteArray);
protected:
    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
