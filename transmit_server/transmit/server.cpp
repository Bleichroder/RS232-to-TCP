#include "server.h"

Server::Server(QObject *parent, int port):QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
}

//有新的连接建立时生成新的socket并存到列表中
void Server::incomingConnection(int socketDescriptor)
{
    TcpClientSocket *tcpClientSocket = new TcpClientSocket(this);
    connect(tcpClientSocket, SIGNAL(sendserial(QByteArray)), this, SLOT(slotsendserial(QByteArray)));
    connect(tcpClientSocket, SIGNAL(disconnected(int)), this, SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    tcpClientSocketList.append(tcpClientSocket);
}

//连接断开时从socket列表中删除记录的socket
void Server::slotDisconnected(int descriptor)
{
    for(int i = 0; i < tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            tcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}

//当收到中控发来的串口命令时向tcpclient转发命令
void Server::sendCmd(QString cmd, int n)
{
    for(int i = 0; i < tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);

        if(item->socketDescriptor() == Util::des){
            QTextStream out(Util::file);
            out << "send tcp data to client : " << cmd << "\n";
            char *msg;
            QByteArray ba = cmd.toUtf8();
            msg = ba.data();
            item->write(msg);
        }
        //qDebug() << item->peerAddress().toIPv4Address();
        //qDebug() << ipv4 << ";" << ip;
        //qDebug() << cmd;
    }
}

void Server::slotsendserial(QByteArray msg)
{
    emit sendserial(msg);
}

