#include "service.h"
#include "chess.h"
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QFont>
#include <QTcpServer>//监听套节字
#include <QTcpSocket>//通信套节字
Service::Service(QWidget *parent) : QMainWindow(parent)
{
//设置窗口
    //设置主窗口标题
    setWindowTitle("六子棋Service");
    //设置窗口的固定大小
    setFixedSize(800,600);
    setWindowIcon(QIcon(":/res/003.png"));

    //设置返回按钮
        QPushButton *btn = new QPushButton;
        btn->setParent(this);
        //设置返回按钮的大小
        btn->setFixedSize(80,40);
        //设置返回按钮上显示的内容
        btn->setText("返回");
        //移动返回按钮
        btn->move(this->width() - btn->width() , this->height() - btn->height());
        //点击了返回按钮，发送一个自定义信号，使主场景可以执行该场景的关闭和主场景的显示
        connect(btn,&QPushButton::clicked,this,[=](){
           emit this->btnclicked();
        });

     //监听套接字，指定父对象，让其自动回收空间
     tcpServer =new QTcpServer(this);
     tcpServer->listen(QHostAddress::Any,8888);

     connect(tcpServer,&QTcpServer::newConnection,[=](){
          qDebug()<<2;
         //取出建立好链接的套接字
         tcpScoket =tcpServer->nextPendingConnection();
         //获取对方的IP和端口
         QString ip=tcpScoket->peerAddress().toString();
         qint16  port =tcpScoket->peerPort();
         QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);
         qDebug()<<temp;


         connect(tcpScoket,&QTcpSocket::readyRead,[=](){
             //从通信套接字中取出内容
             if(X1)       //判断x坐标有没有接收，先接受x后接收y
             {
                 QByteArray array=tcpScoket->readAll();
                 X=QString(array).toInt();
             }
             else
             {
                 QByteArray array=tcpScoket->readAll();
                 Y=QString(array).toInt();
             }
         });
     });




}





void Service::paintEvent(QPaintEvent *)
{

//绘制棋盘
    QPainter painter(this);
    QBrush brush(QColor(210,175,142));
    painter.setBrush(brush);
    painter.drawRect(QRect(0,0,800,600));

    //绘制横线
    QPen  pen (QColor(0,0,0));
    painter.setPen(pen);
    int b=50;
    for(int i=0;i<21;i++)
    {
        painter.drawLine(QPoint(30,25*i+b),QPoint(530,25*i+b));
    }
    //绘制竖线
    b=150;
    for(int i=0;i<21;i++)
    {
        painter.drawLine(QPoint(i*25+30,50),QPoint(i*25+30,550));
    }
}




void  Service::mousePressEvent(QMouseEvent *ev)
{
    if(tcpServer!=NULL)
    {
        QString str1=QString::number(x);
        QString str2=QString::number(y);
        //给对方发送数据，使用的套接字是tcpSocket
        tcpScoket->write(str1.toUtf8().data());
        tcpScoket->write(str2.toUtf8().data());
    }
}
