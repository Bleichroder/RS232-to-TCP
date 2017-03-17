#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include "util.h"

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent = 0);

signals:
    void sendserial(QByteArray);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // TCPCLIENTSOCKET_H
