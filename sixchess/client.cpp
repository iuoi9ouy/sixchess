#include "client.h"
#include "ui_client.h"
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
#include <QHostAddress>
Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

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




            //分配空间，指定父对象
            tcpSocket=new QTcpSocket(this);
            connect(tcpSocket,&QTcpSocket::connected,[=](){
                qDebug()<<"连接成功";
            });


}

Client::~Client()
{
    delete ui;
}






void Client::paintEvent(QPaintEvent *)
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

void Client::on_btn_clicked()
{
    //获取服务器的ip和端口
    QString ip =ui->lineEditip->text();
    qint16 port =ui->lineEditport->text().toInt();

    //主动和服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ip),port);


    qDebug()<<"开始连接";
}
