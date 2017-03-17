#include "tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QDialog(parent)
{
    QIcon icon(":\\color.png");
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(icon);
    systemTray->setToolTip("tcp_server");
    quitAct = new QAction("Quit Application", this);
    connect(quitAct, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    pContextMenu = new QMenu(this);
    pContextMenu->addAction(quitAct);
    systemTray->setContextMenu(pContextMenu);
    systemTray->show();

    //建立一个TcpServer监听端口6666
    port = 6666;
    server = new Server(this, port);
    connect(this, SIGNAL(sendCmd(QString, int)), server, SLOT(sendCmd(QString,int)));
    connect(server, SIGNAL(sendserial(QByteArray)), this, SLOT(serialWrite(QByteArray)));
    initSeialPort();
    content = QByteArray();

    Util::file->open(QIODevice::WriteOnly | QIODevice::Text);
}

TcpServer::~TcpServer()
{
    Util::file->close();
}

//初始化串口
void TcpServer::initSeialPort()
{
    connect(&serial,SIGNAL(readyRead()),this,SLOT(serialRead()));   //连接槽

    //获取计算机上所有串口
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        QMessageBox::information(this, "warning", tr("没有可用串口"));
        return;
    }

    //选择tcp_server需要的COM口
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

//读取串口收到的消息，需要自己生成一个buffer存数据
void TcpServer::serialRead()
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
            emit sendCmd(str, datal + 4);
            QTextStream out(Util::file);
            out << "receive serial data from center control : " << str << "\n";
            content = QByteArray();
        }
        else if((content.length() - 4) > datal)
        {
            content = content.mid(0, datal + 4);
            QTextCodec *tc = QTextCodec::codecForName("UTF8");
            QString str = tc->toUnicode(content);
            emit sendCmd(str, datal + 4);
            QTextStream out(Util::file);
            out << "receive serial data from center control : " << str << "\n";
            content = QByteArray();
        }
    }
}

void TcpServer::serialWrite(QByteArray msg)
{
    QTextStream out(Util::file);
    out << "write serial data to center control : " << msg << "\n";
    char *sendmsg = msg.data();
    serial.write(sendmsg);
}
