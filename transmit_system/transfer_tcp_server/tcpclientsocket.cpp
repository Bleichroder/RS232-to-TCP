#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

//与客户端建立的socket连接收到消息
void TcpClientSocket::dataReceived()
{
    while(bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(bytesAvailable());
        read(datagram.data(), datagram.size());
        QTextCodec *tc = QTextCodec::codecForName("UTF8");
        QString msg = tc->toUnicode(datagram);
        QTextStream out(Util::file);
        out << "receive tcp data from client : " << msg << "\n";
        QByteArray maxwallmsg = msg.toUtf8();
        emit sendserial(maxwallmsg);
        //qDebug() << msg;
    }
}

//与客户端建立的socket连接断开时, 通知server
//删除掉socket列表中的项
void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
