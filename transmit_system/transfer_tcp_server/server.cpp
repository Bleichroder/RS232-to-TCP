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

       //提取中控指令中的目标机器ip
       QString ipv4 = cmd.mid(4, 8);
       quint32 ip = quint32(ipv4.toUInt(0, 16));
       if(ip == item->peerAddress().toIPv4Address())
       {
           cmd = cmd.mid(12, n-12);
           item->write(cmd.toLatin1(), n-12);
           QTextStream out(Util::file);
           out << "send tcp data to client : " << cmd << "\n";
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
