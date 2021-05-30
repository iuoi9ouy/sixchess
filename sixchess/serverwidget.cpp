﻿#include "serverwidget.h"
#include "ui_serverwidget.h"
#include "chess.h"
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QFont>
#include <QDebug>
ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    //设置窗口
        //设置主窗口标题
        setWindowTitle("六子棋Service");
        //设置窗口的固定大小
        setFixedSize(800,600);
        setWindowIcon(QIcon(":/res/003.png"));

        for (int i=0 ;i<21;i++)
        {
            for(int j=0;j<21;j++)
                array[i][j]=0;
        }

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
    tcpServer=new QTcpServer(this);

    tcpServer->listen(QHostAddress::Any,8888);

    connect(tcpServer,&QTcpServer::newConnection,[=](){
        //取出建立好连接的套接字
        tcpSocket=tcpServer->nextPendingConnection();

        //获取对方的IP和端口
        QString ip =tcpSocket->peerAddress().toString();
        connect(tcpSocket, &QTcpSocket::readyRead,this,&ServerWidget::xiaqi);
        connect(tcpSocket, &QTcpSocket::readyRead,[=](){

        });
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}


void ServerWidget::paintEvent(QPaintEvent *)
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








void  ServerWidget::mousePressEvent(QMouseEvent *ev)
{

    if(pix&&(!jieshu))       //如果pix=0则不进入，因为此时还没选择棋子
    {
        if(pix==1)    //选择了黑子
        {
            if(heizi)   //黑子拥有下子权限，本次点击可以下子
            {

                heizi=false;
                //获取鼠标点击位置的坐标
                y=(ev->x()-30+12.5)/25;
                x=(ev->y()-50+12.5)/25;
                if(((x>=0&&x<=20)&&(y>=0&&y<=20))&&(!array[x][y]))   //先判断鼠标点击的位置在范围内且没有下子
                {
                    array[x][y]=1;
                    QString str=":/res/002.png";
                    Chess[x][y]=new chess(str);
                    Chess[x][y]->setParent(this);
                    Chess[x][y]->move(30+25*y-10,50+25*x-10);
                    Chess[x][y]->show();
                    //传递所下子的位置
                    QString str1 =QString::number((10+x)*100+10+y);
                    tcpSocket->write(str1.toUtf8().data());
                    if(panduan())
                    {

                        jieshu=true;
                        QString str="黑子胜利";
                        label= new QLabel(this);
                        label->setText(str);
                        label->setFont(QFont(str,29,QFont::Bold));
                        label ->setStyleSheet("color:red;");
                        label->setFixedSize(200,100);
                        label->move(180,250);
                        label->show();
                    }
                    if(!jieshu)
                    {
                        //判断是否和局
                        bool B=true;
                        for(int i=0;i<20;i++)
                        {
                            for(int j=0;j<20;j++)
                            {
                                if(array[i][j]==0)
                                    B=false;
                            }
                        }
                        if(B)
                        {
                            jieshu=true;
                            QString str="和局";
                            label= new QLabel(this);
                            label->setText(str);
                            label->setFont(QFont(str,29,QFont::Bold));
                            label ->setStyleSheet("color:red;");
                            label->setFixedSize(200,100);
                            label->move(300,250);
                            label->show();
                        }
                    }
                }
            }
        }
        if(pix==2)    //选择了白子
        {
            if(baizi)   //白子拥有下子权限，本次点击可以下子
            {
                baizi=false;
                //获取鼠标点击位置的坐标
                y=(ev->x()-30+12.5)/25;
                x=(ev->y()-50+12.5)/25;
                if(((x>=0&&x<=20)&&(y>=0&&y<=20))&&(!array[x][y]))   //先判断鼠标点击的位置在范围内，且没下子
                {
                    array[x][y]=pix;
                    QString str=":/res/004.png";
                    Chess[x][y]=new chess(str);
                    Chess[x][y]->setParent(this);
                    Chess[x][y]->move(30+25*y-10,50+25*x-10);
                    Chess[x][y]->show();
                    //传递所下子的位置
                    QString str1 =QString::number((10+x)*100+10+y);
                    tcpSocket->write(str1.toUtf8().data());
                    if(panduan())
                    {
                        jieshu=true;
                        QString str="白子胜利";
                        label= new QLabel(this);
                        label->setText(str);
                        label->setFont(QFont(str,29,QFont::Bold));
                        label ->setStyleSheet("color:red;");
                        label->setFixedSize(200,100);
                        label->move(180,250);
                        label->show();
                    }
                    if(!jieshu)
                    {
                        //判断是否和局
                        bool B=true;
                        for(int i=0;i<20;i++)
                        {
                            for(int j=0;j<20;j++)
                            {
                                if(array[i][j]==0)
                                    B=false;
                            }
                        }
                        if(B)
                        {
                            jieshu=true;
                            QString str="和局";
                            label= new QLabel(this);
                            label->setText(str);
                            label->setFont(QFont(str,29,QFont::Bold));
                            label ->setStyleSheet("color:red;");
                            label->setFixedSize(200,100);
                            label->move(300,250);
                            label->show();
                        }
                    }
                }
            }
        }
    }
}


void   ServerWidget::xiaqi()
{
    if(pix==0)
    {
        //从通信套接字中取出内容
        QByteArray array1 = tcpSocket->readAll();
        pix=array1.toInt();
        if(pix==1)
        {
            heizi=true;
        }
    }
    else
    {
         //接收棋子的坐标
         QByteArray array1 = tcpSocket->readAll();
         X=array1.toInt();
         Y=X%100-10;
         X=X/100-10;
         if(pix==2)          //对方下的是黑子
         {
             array[X][Y]=1;
             QString str=":/res/002.png";
             Chess[X][Y]=new chess(str);
             Chess[X][Y]->setParent(this);
             Chess[X][Y]->move(30+25*Y-10,50+25*X-10);
             Chess[X][Y]->show();
             if(panduan())
             {
                 jieshu=true;
                 QString str="黑子胜利";
                 label= new QLabel(this);
                 label->setText(str);
                 label->setFont(QFont(str,29,QFont::Bold));
                 label ->setStyleSheet("color:red;");
                 label->setFixedSize(200,100);
                 label->move(180,250);
                 label->show();
             }
            else
             {
                 if(Panduan(X,Y))
                 {
                     jieshu=true;
                     QString str1="白子胜利";
                     label= new QLabel(this);
                     label->setText(str1);
                     label->setFont(QFont(str1,29,QFont::Bold));
                     label ->setStyleSheet("color:red;");
                     label->setFixedSize(200,100);
                     label->move(180,250);
                     label->show();
                 }
                 if(!jieshu)
                 {
                     //判断是否和局
                     bool B=true;
                     for(int i=0;i<20;i++)
                     {
                         for(int j=0;j<20;j++)
                         {
                             if(array[i][j]==0)
                                 B=false;
                         }
                     }
                     if(B)
                     {
                         jieshu=true;
                         QString str="和局";
                         label= new QLabel(this);
                         label->setText(str);
                         label->setFont(QFont(str,29,QFont::Bold));
                         label ->setStyleSheet("color:red;");
                         label->setFixedSize(200,100);
                         label->move(300,250);
                         label->show();
                     }
                 }
             }
         }
         if(pix==1)                 //对方下的是白子
         {
             array[X][Y]=2;
             QString str=":/res/004.png";
             Chess[X][Y]=new chess(str);
             Chess[X][Y]->setParent(this);
             Chess[X][Y]->move(30+25*Y-10,50+25*X-10);
             Chess[X][Y]->show();
             if(panduan())
             {
                 jieshu=true;
                 QString str1="白子胜利";
                 label= new QLabel(this);
                 label->setText(str1);
                 label->setFont(QFont(str1,29,QFont::Bold));
                 label ->setStyleSheet("color:red;");
                 label->setFixedSize(200,100);
                 label->move(180,250);
                 label->show();
             }
             if(!jieshu)
             {
                 //判断是否和局
                 bool B=true;
                 for(int i=0;i<20;i++)
                 {
                     for(int j=0;j<20;j++)
                     {
                         if(array[i][j]==0)
                             B=false;
                     }
                 }
                 if(B)
                 {
                     jieshu=true;
                     QString str="和局";
                     label= new QLabel(this);
                     label->setText(str);
                     label->setFont(QFont(str,29,QFont::Bold));
                     label ->setStyleSheet("color:red;");
                     label->setFixedSize(200,100);
                     label->move(300,250);
                     label->show();
                 }
             }
         }
         if(pix==1)
         {
             heizi=true;
         }
         if(pix==2)
         {
             baizi=true;
         }
    }
}



//判断是否胜利
bool  ServerWidget::panduan()
{
    bool t=false;//默认为未胜利，为true时胜利
    int  k=0;  //k用来存储遍历时该格的数据
    for(int i=0;i<21;i++)
    {
        for(int j=0;j<21;j++)
        {
            k=array[i][j];
            if(k==0)
            {
                continue;
            }
            //对该格的四个方向进行判断 右 下 右下 左下
            int a=0,b=0;
            while(j-a-1>=0&&array[i][j-a-1]==k)     //对水平方向进行判断
            {                
                a++;
            }
            while (j+b+1<21&&array[i][j+b+1]==k)
            {
                b++;
            }
            if(a+b==5)
            {
                t=true;
                return t;
            }

             a=0;b=0;
            while(i-a-1>=0&&array[i-a-1][j]==k)     //对竖直方向进行判断
            {
                a++;
            }
            while (i+b+1<21&&array[i+b+1][j]==k)
            {
                b++;
            }
            if(a+b==5)
            {
                t=true;
                return t;
            }

            a=0;b=0;
           while((i-a-1>=0&&b-a-1>=0)&&array[i-a-1][j-a-1]==k)     //对右下方向进行判断
           {
               a++;
           }
           while ((i+b+1<21&&j+b+1<21)&&array[i+b+1][j+b+1]==k)
           {
               b++;
           }
           if(a+b==5)
           {
               t=true;
               return t;
           }

           a=0;b=0;
          while((i-a-1>=0&&b+a+1<21)&&array[i-a-1][j+a+1]==k)     //对左下方向进行判断
          {
              a++;
          }
          while ((i+b+1<21&&j-b-1>=0)&&array[i+b+1][j-b-1]==k)
          {
              b++;
          }
          if(a+b==5)
          {
              t=true;
              return t;
          }
        }
    }
    //如果遍历完整个数组都没发现胜利 返回false
    return t;
}




//判断是否禁手
bool ServerWidget::Panduan(int A,int B)
{
//先判断五五禁手
    int a=0,b=0;
    int T[4]={0,0,0,0};         //用来判断4条线上有无符合条件的  符合则置1   T[0]竖直  T[1]水平 T[2]右下斜对角线 T[3]左下斜对角线
    //判断竖直方向
    while(B-a-1>=0&&array[A][B-a-1]==1)
    {
        a++;
    }
    while (B+b+1<21&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if((B-a-1>=0&&B+b+1<21)&&(array[A][B-a-1]==0||array[A][B+b+1]==0))
                T[0]=1;
    }
    if(a+b==3)
    {
        if(B-a-2>=0)
        {
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1)
                T[0]=1;
        }
        if(B+b+2<21)
        {
            if(array[A][B+b+1]==0&&array[A][B+b+2]==1)
                T[0]=1;
        }
    }
    if(a+b==0)
    {
        if(B-a-5>=0)
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1&&array[A][B-a-3]==1&&array[A][B-a-4]==1&&array[A][B-a-5]==1)
            {
                T[0]=1;
            }
        if(B+b+5<21)
            if(array[A][B+b+1]==0&&array[A][B+b+2]==1&&array[A][B+b+3]==1&&array[A][B+b+4]==1&&array[A][B+b+5]==1)
            {
                T[0]=1;
            }
    }
    if(a+b==1)
    {
        if(B-a-3>=0)
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1&&array[A][B-a-3]==1)
            {
                T[0]=1;
            }
        if(B+b+3<21)
            if(array[A][B+b+1]==0&&array[A][B+b+2]==1&&array[A][B+b+3]==1)
            {
                T[0]=1;
            }
    }
    a=0;b=0;
    //判断水平方向
    while(A-a-1>=0&&array[A-a-1][B]==1)
    {
        a++;
    }
    while (A+b+1<21&&array[A+b+1][B]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if((A-a-1>=0&&A+b+1<21)&&(array[A-a-1][B]==0||array[A+b+1][B]==0))
                T[1]=1;
    }
    if(a+b==3)
    {
        if(A-a-2>=0)
        {
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1)
                T[1]=1;
        }
        if(A+b+2<21)
        {
            if(array[A+b+1][B]==0&&array[A+b+2][B]==1)
                T[1]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0)
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1&&array[A-a-3][B]==1&&array[A-a-4][B]==1&&array[A-a-5][B]==1)
            {
                T[1]=1;
            }
        if(A+b+5<21)
            if(array[A+b+1][B]==0&&array[A+b+2][B]==1&&array[A+b+3][B]==1&&array[A+b+4][B]==1&&array[A+b+5][B]==1)
            {
                T[1]=1;
            }
    }
    if(a+b==1)
    {
        if(A-a-3>=0)
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1&&array[A-a-3][B]==1)
            {
                T[1]=1;
            }
        if(A+b+3<21)
            if(array[A+b+1][B]==0&&array[A+b+2][B]==1&&array[A+b+3][B]==1)
            {
                T[1]=1;
            }
    }
    a=0;b=0;
    //判断右下斜对角线方向
    while((A-a-1>=0&&B-a-1>=0)&&array[A-a-1][B-a-1]==1)
    {
        a++;
    }
    while ((A+b+1<21&&B+b+1<21)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if(((A-a-1>=0&&B-a-1>=0)&&(A+b+1<21&&B+b+1<21))&&(array[A-a-1][B-a-1]==0||array[A+b+1][B+b+1]==0))
                T[2]=1;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B-a-2>=0)
        {
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1)
                T[2]=1;
        }
        if(A+b+2<21&&B+b+2<21)
        {
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1)
                T[2]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0&&B-a-5>=0)
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1&&array[A-a-3][B-a-3]==1&&array[A-a-4][B-a-4]==1&&array[A-a-5][B-a-5]==1)
            {
                T[2]=1;
            }
        if(A+b+5<21&&B+b+5<21)
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==1&&array[A+b+4][B+b+4]==1&&array[A+b+5][B+b+5]==1)
            {
                T[2]=1;
            }
    }
    if(a+b==1)
    {
        if(A-a-3>=0&&B-a-3>=0)
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1&&array[A-a-3][B-a-3]==1)
            {
                T[2]=1;
            }
        if(A+b+3<21&&B+b+3<21)
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==1)
            {
               T[2]=1;
            }
    }
    a=0;b=0;
    //判断左下斜对角线方向
    while((A- a-1>=0&&B+a+1<21)&&array[A-a-1][B+a+1]==1)
    {
        a++;
    }
    while ((A+b+1<21&&B-b-1>=0)&&array[A+b+1][B-+b-1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if(((A- a-1>=0&&B+a+1<21)&&(A+b+1<21&&B-b-1>=0))&&(array[A-a-1][B+a+1]==0||array[A+b+1][B-+b-1]==0))
                T[3]=1;
    }

    if(a+b==3)
    {
        if(A-a-2>=0&&B+a+2<21)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1)
                T[3]=1;
        }
        if(A+b+2<21&&B-b-2>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1)
                T[3]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0&&B+a+5<21)
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==1&&array[A-a-5][B+a+5]==1)
            {
                T[3]=1;
            }
        if(A+b+5<21&&B-b-5>=0)
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==1&&array[A+b+5][B-b-5]==1)
            {
                T[3]=1;
            }
    }
    if(a+b==1)
    {
        if(A-a-3>=0&&B+a+3<21)
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1)
            {
                T[3]=1;
            }
        if(A+b+3<21&&B-b-3>=0)
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1)
            {
                T[3]=1;
            }
    }
    int t=T[0]+T[1]+T[2]+T[3];
    if(t>1)
        return true;
//判断四四禁手
    a=0;b=0;
    for(int i=0;i<4;i++)
    {
        T[i]=0;
    }
    //判断竖直方向
    while(B-a-1>=0&&array[A][B-a-1]==1)
    {
        a++;
    }
    while (B+b+1<21&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(B-a-2>=0)
            if(B+b+1<21)
                if(array[A][B-a-1]==0&&array[A][B-a-2]==0&&array[A][B+b+1]==0)
                    T[0]=1;
        if(B-a-1>=0)
            if(B+b+2<21)
                if(array[A][B-a-1]==0&&array[A][B+b+1]==0&&array[A][B+b+2]==0)
                    T[0]=1;
    }
    if(a+b==2)
    {
        if(B-a-3>=0)
        {
            if((array[A][B-a-1]==0)&&(array[A][B-a-2]==1)&&(array[A][B-a-3]==0))
            {
              if(B+b+1<21&&array[A][B+b+1]==0)
                  T[0]=1;
            }

        }
        if(B-a-1>=0&&array[A][B-a-1]==0)
        {
          if(B+b+3<21)
          {
              if(array[A][B+b+1]==0&&array[A][B+b+2]==1&&array[A][B+b+3]==0)
                  T[0]=1;
          }
        }
    }    
    if(a+b==1)
    {
        if(B-a-4>=0)
        {
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1&&array[A][B-a-3]==1&&array[A][B-a-4]==0)
                if(B+b+1<21&&array[A][B+b+1]==0)
                    T[0]=1;

        }

        if(B+b+4<21)
        {
            if(array[A][B+b+1]==0&&array[A][B+b+2]==1&&array[A][B+b+3]==1&&array[A][B+b+4]==0)
                if(B-a-1>=0&&array[A][B-a-1]==0)
                    T[0]=1;
        }
    }
    if(a+b==0)
    {
        if(B-a-5>=0)
        {
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1&&array[A][B-a-3]==1&&array[A][B-a-4]==1&&array[A][B-a-5]==0)
            {
                if(B+b+1<21&&array[A][B+b+1]==0)
                    T[0]=1;
            }
        }
        if(B+b+5<21)
        {
            if(array[A][B+b+1]==0&&array[A][B+b+2]==1&&array[A][B+b+3]==1&&array[A][B+b+4]==1&&array[A][B+b+5]==0)
            {
                if(B-a-1>=0&&array[A][B-a-1]==0)
                    T[0]=1;
            }
        }
    }
    a=0;b=0;
    //判断水平方向
    while(A-a-1>=0&&array[A-a-1][B]==1)
    {
        a++;
    }
    while (A+b+1<21&&array[A+b+1][B]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0)
            if(A+b+1<21)
                if(array[A-a-1][B]==0&&array[A-a-2][B]==0&&array[A+b+1][B]==0)
                    T[1]=1;
        if(A-a-1>=0)
            if(A+b+2<21)
                if(array[A-a-1][B]==0&&array[A+b+1][B]==0&&array[A+b+2][B]==0)
                    T[1]=1;
    }
    if(a+b==2)
    {
        if(A-a-3>=0)
        {
            if((array[A-a-1][B]==0)&&(array[A-a-2][B]==1)&&(array[A-a-3][B]==0))
            {
              if(A+b+1<21&&array[A+b+1][B]==0)
                  T[1]=1;
            }
        }

        if(A-a-1>=0&&array[A-a-1][B]==0)
        {
          if(A+b+3<21)
          {
              if(array[A+b+1][B]==0&&array[A+b+2][B]==1&&array[A+b+3][B]==0)
                  T[1]=1;
          }
        }

    }
    if(a+b==1)
    {
        if(A-a-4>=0)
        {
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1&&array[A-a-3][B]==1&&array[A-a-4][B]==0)
                if(A+b+1<21&&array[A+b+1][B]==0)
                    T[1]=1;

        }
        if(A+b+4<21)
        {
            if(array[A+b+1][B]==0&&array[A+b+2][B]==1&&array[A+b+3][B]==1&&array[A+b+4][B]==0)
                if(A-a-1>=0&&array[A-a-1][B]==0)
                    T[1]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0)
        {
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1&&array[A-a-3][B]==1&&array[A-a-4][B]==1&&array[A-a-5][B]==0)
            {
                if(A+b+1<21&&array[A+b+1][B]==0)
                    T[1]=1;
            }
        }
        if(A+b+5<21)
        {
            if(array[A+b+1][B]==0&&array[A+b+2][B]==1&&array[A+b+3][B]==1&&array[A+b+4][B]==1&&array[A+b+5][B]==0)
            {
                if(A-a-1>=0&&array[A-a-1][B]==0)
                    T[1]=1;
            }
        }
    }
    a=0;b=0;
    //判断右下斜对角线方向
    while((A-a-1>=0&&B-a-1>=0)&&array[A-a-1][B-a-1]==1)
    {
        a++;
    }
    while ((A+b+1<21&&B+b+1<21)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B-a-2>=0)
            if(A+b+1<21&&B+b+1<21)
                if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==0&&array[A+b+1][B+b+1]==0)
                    T[2]=1;
        if(A-a-1>=0&&B-a-1>=0)
            if(A+b+2<21&&B+b+2<21)
                if(array[A-a-1][B-a-1]==0&&array[A+b+1][B+b+1]==0&&array[A+b+2][A+b+2]==0)
                    T[2]=1;
    }
    if(a+b==2)
    {
        if(B-a-3>=0&&A-a-3>=0)
        {
            if((array[A-a-1][B-a-1]==0)&&(array[A-a-2][B-a-2]==1)&&(array[A-a-3][B-a-3]==0))
            {
              if((B+b+1<21&&A+b+1<21)&&array[A+b+1][B+b+1]==0)
                  T[2]=1;
            }
        }        
        if((B-a-1>=0&&A-a-1>=0)&&array[A-a-1][B-a-1]==0)
        {
          if(B+b+3<21&&A+b+3<21)
          {
              if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==0)
                  T[2]=1;
          }
        }

    }
    if(a+b==1)
    {
        if(B-a-4>=0&&A-a-4>=0)
        {
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1&&array[A-a-3][B-a-3]==1&&array[A-a-4][B-a-4]==0)
                if((B+b+1<21&&A+b+1<21)&&array[A+b+1][B+b+1]==0)
                    T[2]=1;

        }
        if(A+b+4<21&&B+b+4<21)
        {
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==1&&array[A+b+4][B+b+4]==0)
                if((A-a-1>=0&&B-a-1>=0)&&array[A-a-1][B-a-1]==0)
                    T[2]=1;
        }
    }
    if(a+b==0)
    {
        if(B-a-5>=0&&A-a-5>=0)
        {
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1&&array[A-a-3][B-a-3]==1&&array[A-a-4][B-a-4]==1&&array[A-a-5][B-a-5]==0)
            {
                if((B+b+1<21&&A+b+1<21)&&array[A+b+1][B+b+1]==0)
                    T[2]=1;
            }
        }
        if(B+b+5<21&&A+b+5<21)
        {
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==1&&array[A+b+4][B+b+4]==1&&array[A+b+5][B+b+5]==0)
            {
                if((B-a-1>=0&&A-a-1>=0)&&array[A-a-1][B-a-1]==0)
                    T[2]=1;
            }
        }
    }
    a=0;b=0;
    //判断左下斜对角线方向
    while((A-a-1>=0&&B+a+1<21)&&array[A-a-1][B+a+1]==1)
    {
        a++;
    }
    while ((A+b+1<21&&B-b-1>=0)&&array[A+b+1][B-+b-1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B+a+2<21)
            if(A+b+1<21&&B-b-1>=0)
                if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==0&&array[A+b+1][B-b-1]==0)
                    T[3]=1;
        if(A-a-1>=0&&B+a+1<21)
            if(A+b+2<21&&B-b-2>=0)
                if(array[A-a-1][B+a+1]==0&&array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==0)
                    T[3]=1;
    }
    if(a+b==2)
    {
        if(B+a+3<21&&A-a-3>=0)
        {
            if((array[A-a-1][B+a+1]==0)&&(array[A-a-2][B+a+2]==1)&&(array[A-a-3][B+a+3]==0))
            {
              if((A+b+1<21&&B-b-1>=0)&&array[A+b+1][B-b-1]==0)
                  T[3]=1;
            }
        }
        if((B+a+1<21&&A-a-1>=0)&&array[A-a-1][B+a+1]==0)
        {
          if(A+b+3<21&&B-b-3>=0)
          {
              if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==0)
                  T[3]=1;
          }
        }
    }
    if(a+b==1)
    {
        if(B+a+4<21&&A-a-4>=0)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==0)
                if((B-b-1<21&&A+b+1>=0)&&array[A+b+1][B-b-1]==0)
                    T[0]=1;

        }
        if(A+b+4<21&&B-b-4>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==0)
                if((A-a-1>=0&&B+a+1<21)&&array[A-a-1][B+a+1]==0)
                    T[0]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0&&B+a+5<21)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==1&&array[A-a-5][B+a+5]==0)
            {
                if((A+b+1<21&&B-b-1>=0)&&array[A+b+1][B-b-1]==0)
                    T[1]=1;
            }
        }
        if(A+b+5<21&&B-b-1>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==1&&array[A+b+5][B-b-5]==0)
            {
                if((A-a-1>=0&&B+a+1<21)&&array[A-a-1][B+a+1]==0)
                    T[1]=1;
            }
        }
    }
    t=T[0]+T[1]+T[2]+T[3];
    if(t>1)
        return true;


    //判断长连以及复杂禁手
    a=0;b=0;
    for(int i=0;i<4;i++)
    {
        T[i]=0;
    }
    //对该格的四个方向进行判断 右 下 右下 左下
    while(B-a-1>=0&&array[A][B-a-1]==1)     //对水平方向进行判断
    {
        a++;
    }
    while (B+b+1<21&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b>5){T[0]=2;}

     a=0;b=0;
    while(A-a-1>=0&&array[A-a-1][B]==1)     //对竖直方向进行判断
    {
        a++;
    }
    while (A+b+1<21&&array[A+b+1][B]==1)
    {
        b++;
    }
     if(a+b>5){T[0]=2;}

    a=0;b=0;
    while((A-a-1>=0&&b-a-1>=0)&&array[A-a-1][B-a-1]==1)     //对右下方向进行判断
    {
        a++;
    }
    while ((A+b+1<21&&B+b+1<21)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b>5){T[0]=2;}

    a=0;b=0;
    while((A-a-1>=0&&b+a+1<21)&&array[A-a-1][B+a+1]==1)     //对左下方向进行判断
    {
       a++;
    }
    while ((A+b+1<21&&B-b-1>=0)&&array[A+b+1][B-b-1]==1)
    {
       b++;
    }
    if(a+b>5){T[0]=2;}
    t=T[0]+T[1]+T[2]+T[3];
    if(t>1)
    {
        return true;
    }
    return  false;
}
