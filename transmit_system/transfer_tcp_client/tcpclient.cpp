#include "tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QDialog(parent)
{
    QIcon icon(":\\color.png");
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(icon);
    systemTray->setToolTip("tcp_client");
    quitAct = new QAction("Quit Application", this);
    connAct = new QAction(tr("连接服务器"), this);
    disconnAct = new QAction(tr("与服务器断开连接"), this);

    connect(connAct, SIGNAL(triggered(bool)), this, SLOT(connClick()));
    connect(disconnAct, SIGNAL(triggered(bool)), this, SLOT(disconnClick()));
    connect(quitAct, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    pContextMenu = new QMenu(this);
    pContextMenu->addAction(quitAct);
    pContextMenu->addAction(connAct);
    pContextMenu->addAction(disconnAct);
    systemTray->setContextMenu(pContextMenu);
    systemTray->show();
    hasconnected = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    //默认连接服务器
    serverIP = new QHostAddress();
    QSettings settings("config.ini", QSettings::IniFormat);
    QString ip = settings.value("DevOption/ServerIP").toString();
    if(ip == "")
    {
        QMessageBox::information(this, "warnning", "No ServerIP setting in config.ini");
        return;
    }
    serverIP->setAddress(ip);
    port = 6666;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    tcpSocket->connectToHost(*serverIP, port);
    initSeialPort();
    content = QByteArray();

    file = new QFile("log.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);

    timer->start(20000);
}

TcpClient::~TcpClient()
{
    file->close();
}

//托盘选择连接服务器
void TcpClient::connClick()
{
    tcpSocket->connectToHost(*serverIP, port);
}

//托盘选择与服务器断开连接
void TcpClient::disconnClick()
{
    tcpSocket->disconnectFromHost();
}

//成功连接到服务器
void TcpClient::slotConnected()
{
    hasconnected = true;
    connAct->setEnabled(false);
    disconnAct->setEnabled(true);
    systemTray->showMessage(tr("提示"), tr("已成功连接至服务器"), QSystemTrayIcon::Information, 5000);
}

//成功与服务器断开
void TcpClient::slotDisconnected()
{
    hasconnected = false;
    connAct->setEnabled(true);
    disconnAct->setEnabled(false);
    systemTray->showMessage(tr("提示"), tr("已断开与服务器的连接"), QSystemTrayIcon::Information, 5000);
}

//与服务器连接的socket收到数据
void TcpClient::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(), datagram.size());
        QTextCodec *tc = QTextCodec::codecForName("UTF8");
        QString msg = tc->toUnicode(datagram);
        QTextStream out(file);
        out << "receive tcp data from server : " << msg << "\n";
        QByteArray maxwallmsg = msg.toUtf8();
        serial.write(maxwallmsg);
        out << "send serial data to maxwall server : " << maxwallmsg << "\n";
        //qDebug() << msg;
    }
}

//串口收到maxwall返回值之后通过socket向服务器发送数据
void TcpClient::receive(QString re, int n)
{
    QTextStream out(file);
    out << "send tcp data to server : " << re << "\n";
    char *msg;
    QByteArray ba = re.toUtf8();
    msg = ba.data();
    tcpSocket->write(ba, n);
    //qDebug() << re;
}

//初始化串口
void TcpClient::initSeialPort()
{
    connect(&serial,SIGNAL(readyRead()),this,SLOT(serialRead()));   //连接槽

    //获取计算机上所有串口并添加到comboBox中
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        QMessageBox::information(this, "warning", tr("没有可用串口"));
        return;
    }
    QSerialPortInfo usecom;
    QSettings settings("config.ini", QSettings::IniFormat);
    QString com = settings.value("DevOption/COM").toString();
    if(com == "")
    {
        QMessageBox::information(this, "warnning", "No COM setting in config.ini");
        return;
    }

    foreach (QSerialPortInfo info, infos) {
        if(info.portName() == com)
        {
            usecom = info;
            break;
        }
        //qDebug() << info.portName();
    }

    serial.close();
    serial.setPort(usecom);
    serial.open(QIODevice::ReadWrite);          //读写打开
    serial.setBaudRate(QSerialPort::Baud9600);  //波特率
    serial.setDataBits(QSerialPort::Data8);     //数据位
    serial.setParity(QSerialPort::NoParity);    //无奇偶校验
    serial.setStopBits(QSerialPort::OneStop);   //无停止位
    serial.setFlowControl(QSerialPort::NoFlowControl);  //无控制
}

//串口收到maxwall返回值
void TcpClient::serialRead()
{
    QByteArray qa = serial.readAll();
    content.append(qa);
    if(content.length() > 4)
    {
        QString datalen = content.mid(0, 4);
        int datal = datalen.toInt();
        if((content.length() - 4) == datal)
        {
            QTextCodec *tc = QTextCodec::codecForName("UTF8");
            QString str = tc->toUnicode(content);
            receive(str, datal + 4);
            QTextStream out(file);
            out << "receive serial data from maxwall server : " << str << "\n";
            content = QByteArray();
        }
        else if((content.length() - 4) > datal)
        {
            content = content.mid(0, datal + 4);
            QTextCodec *tc = QTextCodec::codecForName("UTF8");
            QString str = tc->toUnicode(content);
            receive(str, datal + 4);
            QTextStream out(file);
            out << "receive serial data from maxwall server : " << str << "\n";
            content = QByteArray();
        }
    }
}

void TcpClient::timeout()
{
    if(!hasconnected)
        tcpSocket->connectToHost(*serverIP, port);
}
