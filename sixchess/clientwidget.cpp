#include "clientwidget.h"
#include "ui_clientwidget.h"
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
#include <QTimer>
#include <QTime>
ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    //分配空间，指定父对象
    tcpSocket =new QTcpSocket(this);
    timer=new QTimer(this);//用于自动下棋
    //设置窗口
        //设置主窗口标题
        setWindowTitle("六子棋Client");
        //设置窗口的固定大小
        setFixedSize(800,600);
        setWindowIcon(QIcon(":/res/003.png"));
        //对数组进行赋初值为0
        for (int i=0;i<21;i++)
        {
            for(int j=0;j<21;j++)
            {
                array[i][j]=0;
                Chess[i][j]=NULL;
            }
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

            connect(tcpSocket, &QTcpSocket::readyRead,this,&ClientWidget::xiaqi);

            connect(timer,&QTimer::timeout,this,&ClientWidget::Zidong);
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_ButtonConnect_clicked()
{
    //获取服务器ip和端口
    QString ip=ui->lineEditIP->text();
    qint16 port =ui->lineEditPort->text().toInt();
    k=true;
    //主动和服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ip),port);
}


void ClientWidget::paintEvent(QPaintEvent *)
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



//点击黑子按钮，选择黑子
void ClientWidget::on_Buttonhei_clicked()
{
    if((pix==0&&k)&&duimian)
    {
        pix=1;
        hei=true;
        heizi=true;//黑子获得下子权限
        //给server传递要下的棋子
        QString str =QString::number(2);
        //发送数据
        tcpSocket->write( str.toUtf8().data() );
    }
}


//点击白子按钮，选择白子
void ClientWidget::on_pushButton_2_clicked()
{
    if((pix==0&&k)&&(duimian))
    {
        bai=true;
        pix=2;
        //给server传递要下的棋子
        QString str =QString::number(1);
        //发送数据
        tcpSocket->write( str.toUtf8().data() );
    }
}



//重写鼠标点击事件
void  ClientWidget::mousePressEvent(QMouseEvent *ev)
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
                if(((x>=0&&x<=20)&&(y>=0&&y<=20))&&!array[x][y])   //先判断鼠标点击的位置在范围内，且没下子
                {

                    array[x][y]=pix;
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
                    else
                    {
                        if(Panduan(x,y))
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
                        //判断是否和局
                        bool B=true;
                        for(int i=0;i<21;i++)
                        {
                            for(int j=0;j<21;j++)
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
                            label->move(180,250);
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
                if(((x>=0&&x<=20)&&(y>=0&&y<=20))&&(!array[x][y]))   //先判断鼠标点击的位置在范围内且没有下子
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
                    else
                    {
                        //判断是否和局
                        bool B=true;
                        for(int i=0;i<21;i++)
                        {
                            for(int j=0;j<21;j++)
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
                            label->move(180,250);
                            label->show();
                        }
                    }
                }
            }
        }
    }
}



void   ClientWidget::xiaqi()
{
    if(!duimian)            //第一次传回的信号是对面选择了手动或自动
    {
        QByteArray array1 = tcpSocket->readAll();
        duimian=array1.toInt();
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
                     QString str1="黑子胜利";
                     label= new QLabel(this);
                     label->setText(str1);
                     label->setFont(QFont(str1,29,QFont::Bold));
                     label ->setStyleSheet("color:red;");
                     label->setFixedSize(200,100);
                     label->move(180,250);
                     label->show();
                     if(timer!=NULL)
                     {
                         timer->stop();
                     }
                     qDebug()<<X<<Y;
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
                         if(timer!=NULL)
                         {
                             timer->stop();
                         }
                         qDebug()<<X<<Y<<"禁手";
                     }
                     //判断是否和局
                     bool B=true;
                     for(int i=0;i<21;i++)
                     {
                         for(int j=0;j<21;j++)
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
                         label->move(180,250);
                         label->show();
                         if(timer!=NULL)
                         {
                             timer->stop();
                         }
                     }
                 }
             }
             if(pix==1)         //对方下的是白子
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
                     if(timer!=NULL)
                     {
                         timer->stop();
                     }
                     qDebug()<<X<<Y;
                 }
                 else
                 {
                     //判断是否和局
                     bool B=true;
                     for(int i=0;i<21;i++)
                     {
                         for(int j=0;j<21;j++)
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
                         label->move(180,250);
                         label->show();
                         if(timer!=NULL)
                         {
                             timer->stop();
                         }
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
bool  ClientWidget::panduan()
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
           while((i-a-1>=0&&j-a-1>=0)&&array[i-a-1][j-a-1]==k)     //对右下方向进行判断
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
          while((i-a-1>=0&&j+a+1<21)&&array[i-a-1][j+a+1]==k)     //对左下方向进行判断
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
bool ClientWidget::Panduan(int A,int B)
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
        if(A+b+5<21&&B-b-5>=0)
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
    while((A-a-1>=0&&B-a-1>=0)&&array[A-a-1][B-a-1]==1)     //对右下方向进行判断
    {
        a++;
    }
    while ((A+b+1<21&&B+b+1<21)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b>5){T[0]=2;}

    a=0;b=0;
    while((A-a-1>=0&&B+a+1<21)&&array[A-a-1][B+a+1]==1)     //对左下方向进行判断
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


int ClientWidget::XIAQI()
 {
     int r=0;
     int k=pix;               //k为黑白子
     int a[21][21];
     int t=0;
     //将数组a里面的值全部改为-10
     for(int i=0;i<21;i++)
     {
         for(int j=0;j<21;j++)
         {
          a[i][j]=0;
         }
     }
     //如果数组array 里面为0，则说明该位置还没有下此时在数组a存储评分   把已经下了的排除在外
     for(int i=0;i<21;i++)
     {
         for(int j=0;j<21;j++)
         {
             if(array[i][j]==0)
                  a[i][j]=pingfen(i,j,k);
         }
     }
     //取出数组a里面评分最大的值
     for(int i=0;i<21;i++)
     {
         for(int j=0;j<21;j++)
         {
             if(a[i][j]>t)
                 t=a[i][j];
         }
     }

    bool  l=false;
    for(int i=0;i<21;i++)
    {
        for(int j=0;j<21;j++)
        {
            if(a[i][j]==t)
            {
                r=100*i+j;
                l=true;

            }
            if(l)
                break;
        }
        if(l)
            break;
    }
    return r;
 }

void ClientWidget::on_pushButton_3_clicked()
{
    if((!shoudong)&&(!zidong))
    {
        shoudong=true;
    }
}

void ClientWidget::on_pushButton_clicked()
{
    if((!shoudong)&&(!zidong))
    {
        zidong=true;
        timer->start(500);
    }
}

void ClientWidget::Zidong()
{
    if(heizi&&pix==1)
    {
        if(hei)                     //第一次下黑子
        {
            hei=false;
            heizi=false;
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            int t=qrand()%25;
            x=8+t/5;
            y=8+t%5;
            array[x][y]=1;
            QString str=":/res/002.png";
            chess *Chess=new chess(str);
            Chess->setParent(this);
            Chess->move(30+25*y-10,50+25*x-10);
            Chess->show();
            //传递所下子的位置
            QString str1 =QString::number((10+x)*100+10+y);
            tcpSocket->write(str1.toUtf8().data());
        }
        else
        {
            heizi=false;
            int t=XIAQI();
            x=t/100;
            y=t%100;
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
                QString str1="黑子胜利";
                label= new QLabel(this);
                label->setText(str1);
                label->setFont(QFont(str1,29,QFont::Bold));
                label ->setStyleSheet("color:red;");
                label->setFixedSize(200,100);
                label->move(180,250);
                label->show();
                if(timer!=NULL)
                {
                    timer->stop();
                }
            }
            else
            {
                //判断是否和局
                bool B=true;
                for(int i=0;i<21;i++)
                {
                    for(int j=0;j<21;j++)
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
                    label->move(180,250);
                    label->show();
                    if(timer!=NULL)
                    {
                        timer->stop();
                    }
                }
            }
        }
    }
    if(baizi&&pix==2)
    {
       if(bai)
       {
           bai=false;
           baizi=false;
           qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
           int n=qrand()%9;//随机产生0-8的数
           while (n==4)     //避免下在黑子的位置
           {
               n=qrand()%9+1;
           }
           x=X-1+n/3;
           y=Y-1+n%3;
           array[x][y]=2;
           QString str=":/res/004.png";
           Chess[x][y]=new chess(str);
           Chess[x][y]->setParent(this);
           Chess[x][y]->move(30+25*y-10,50+25*x-10);
           Chess[x][y]->show();
           //传递所下子的位置
           QString str1 =QString::number((10+x)*100+10+y);
           tcpSocket->write(str1.toUtf8().data());
       }
       else
       {
           baizi=false;
           int t=XIAQI();
           x=t/100;
           y=t%100;
           array[x][y]=2;
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
               QString str1="白子胜利";
               label= new QLabel(this);
               label->setText(str1);
               label->setFont(QFont(str1,29,QFont::Bold));
               label ->setStyleSheet("color:red;");
               label->setFixedSize(200,100);
               label->move(180,250);
               label->show();
               if(timer!=NULL)
               {
                   timer->stop();
               }
           }
           else
           {
               //判断是否和局
               bool B=true;
               for(int i=0;i<21;i++)
               {
                   for(int j=0;j<21;j++)
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
                   label->move(180,250);
                   label->show();
                   if(timer!=NULL)
                   {
                       timer->stop();
                   }
               }
           }
       }
    }
}


int  ClientWidget::pingfen(int A,int B,int C)
{
    int a=0;int b=0;
    int sum=0;              //用于统计分数
    int Array1[21][21];     //用于复刻棋盘上的棋子
    for(int i=0;i<21;i++)
    {
        for(int j=0;j<21;j++)
        {
            Array1[i][j]=array[i][j];
        }
    }
    //先假设在棋盘上下一颗字，并统计该格上的分数
    Array1[A][B]=C;
    //如果电脑是黑子要自动排除自己下了禁手的情况
    if(C==1)
    {
        array[A][B]=C;
        if(Panduan(A,B))
        {
             array[A][B]=0;             //调用完判断禁手函数后要将array数组中的值变回来
                    return  -100;
       }
              array[A][B]=0;             //调用完判断禁手函数后要将array数组中的值变回来
    }
    //判断竖直方向
    while(B-a-1>=0&&Array1[A][B-a-1]==C)
    {
        a++;
    }
    while (B+b+1<21&&Array1[A][B+b+1]==C)
    {
        b++;
    }
    if(a+b==5)                  //连成六子
    {
        sum=sum+10000000;
    }
    if(a+b==4)                  //连成五子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+10000;
        else
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //眠五
                sum=sum+10000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //活四
                sum=sum+8000;
            else                                            //眠四
                sum=sum+250;
        else
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //活三
                sum=sum+1800;
            else                                            //眠三
                sum=sum+150;
        else
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+20;
        else
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)                //活单
                sum=sum+20;
            else
                sum=sum+10;
        else
                sum=sum+10;
    }
    a=0;b=0;
    //判断水平方向
    while(A-a-1>=0&&Array1[A-a-1][B]==C)
    {
        a++;
    }
    while(A+b+1<21&&Array1[A+b+1][B]==C)
    {
        b++;
    }
    if(a+b==5)                  //连成六子
    {
        sum=sum+10000000;
    }
    if(a+b==4)                  //连成五子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+20000;
        else
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //眠五
                sum=sum+20000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //活四
                sum=sum+8000;
            else                                            //眠四
                sum=sum+2500;
        else
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //活三
                sum=sum+1800;
            else                                            //眠三
                sum=sum+1500;
        else
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)                //活单
                sum=sum+20;
            else
                sum=sum+10;
        else
                sum=sum+10;
    }
    a=0;b=0;
    //判断右下斜对角线
    while((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==C)
    {
        a++;
    }
    while ((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==C)
    {
        b++;
    }
    if(a+b==5)                  //连成六子
    {
        sum=sum+10000000;
    }
    if(a+b==4)                  //连成五子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)               //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+10000;
        else
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)                 //眠五
                sum=sum+10000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)                 //活四
                sum=sum+8000;
            else                                            //眠四
                sum=sum+2500;
        else
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)                 //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)            //活三
                sum=sum+1800;
            else                                            //眠三
                sum=sum+1500;
        else
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)                 //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)              //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)             //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<21&&B+b+1<21)&&Array1[A+b+1][B+b+1]==0)              //活单
                sum=sum+20;
            else
                sum=sum+10;
        else
                sum=sum+10;
    }
    a=0;b=0;
    //判断左下斜对角线方向
    while((A- a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==C)
    {
        a++;
    }
    while ((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-+b-1]==C)
    {
        b++;
    }
    if(a+b==5)                  //连成六子
    {
        sum=sum+10000000;
    }
    if(a+b==4)                  //连成五子
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)               //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+10000;
        else
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)                 //眠五
               sum=sum+10000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)          //活四
                sum=sum+8000;
            else                                            //眠四
                sum=sum+2500;
        else
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)                 //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)          //活三
                sum=sum+1800;
            else                                            //眠三
                sum=sum+1500;
        else
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)             //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)             //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)           //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<21&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)              //活单
                sum=sum+20;
            else
                sum=sum+10;
        else
                sum=sum+10;
    }

    a=0;
//对堵掉对面棋子进行评分统计,对八个方向进行统计
    int fan=0;
    if(C==1)
    {
        fan=2;
    }
    if(C==2)
    {
        fan=1;
    }
    //对左进行统计
    while (B-a-1>=0&&Array1[A][B-a-1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(B-a-1>=0&&Array1[A][ B-a-1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
        if(B-a-1>=0&&Array1[A][ B-a-1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
        if(B-a-1>=0&&Array1[A][ B-a-1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
        if(B-a-1>=0&&Array1[A][ B-a-1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }

    a=0;
    //对左上进行统计
    while ((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][B-a-1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][ B-a-1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][ B-a-1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][ B-a-1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][ B-a-1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }


    a=0;
    //对上进行统计
    while (A-a-1>=0&&Array1[A-a-1][B]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(A-a-1>=0&&Array1[A-a-1][ B]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
        if(A-a-1>=0&&Array1[A-a-1][ B]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
       if(A-a-1>=0&&Array1[A-a-1][ B]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
        if(A-a-1>=0&&Array1[A-a-1][ B]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }

    a=0;
    //对右上进行统计
    while ((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
       if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
        if((A-a-1>=0&&B+a+1<21)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }


    a=0;
    //对右进行统计
    while (B+a+1<21&&Array1[A][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(B+a+1<21&&Array1[A][ B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if(B+a+1<21&&Array1[A][ B+a+1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
       if(B+a+1<21&&Array1[A][ B+a+1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
         if(B+a+1<21&&Array1[A][ B+a+1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }


    a=0;
    //对右下进行统计
    while ((B+a+1<21&&A+a+1<21)&&Array1[A+a+1][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((B+a+1<21&&A+a+1<21)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if((B+a+1<21&&A+a+1<21)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
        if((B+a+1<21&&A+a+1<21)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
         if((B+a+1<21&&A+a+1<21)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }

    a=0;
    //对下进行统计
    while (A+a+1<21&&Array1[A+a+1][B]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(A+a+1<21&&Array1[A+a+1][ B]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if(A+a+1<21&&Array1[A+a+1][ B]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
        if(A+a+1<21&&Array1[A+a+1][ B]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
         if(A+a+1<21&&Array1[A+a+1][ B]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }


    a=0;
    //对左下进行统计
    while ((B-a-1>=0&&A+a+1<21)&&Array1[A+a+1][B-a-1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((B-a-1>=0&&A+a+1<21)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
      if((B-a-1>=0&&A+a+1<21)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+55000;
        else
            sum=sum+4500;
    }
    if(a==3)
    {
        if((B-a-1>=0&&A+a+1<21)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+5000;
        else
            sum=sum+3000;
    }
    if(a==2)
    {
         if((B-a-1>=0&&A+a+1<21)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+800;
        else
            sum=sum+1;
    }
    if(a==1)
    {
        sum=sum+1;
    }


//对四个方向进行隔着的五子判断
     a=0;b=0;
    //上下方向
    while (A-a-1>=0&&Array1[A-a-1][B]==fan)
    {
        a++;
    }
    while (A+b+1<21&&Array1[A+b+1][B]==fan)
    {
        b++;
    }
    if(a+b==5)
    {
        sum=sum+300000;
    }
    if(a+b==4)
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<21&&Array1[A+b+1][B]==0)
                sum=sum+60000;
    }
    a=0;b=0;
    //左右方向
    while (B-a-1>=0&&Array1[A][B-a-1]==fan)
    {
        a++;
    }
    while (B+b+1<21&&Array1[A][B+b+1]==fan)
    {
        b++;
    }
    if(a+b==5)
    {
        sum=sum+300000;
     }
    if(a+b==4)
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<21&&Array1[A][B+b+1]==0)
                sum=sum+60000;
    }

    a=0;b=0;
    //左上、右下
    while ((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][B-a-1]==fan)
    {
        a++;
    }
    while ((B+b+1<21&&A+b+1<21)&&Array1[A+b+1][B+b+1]==fan)
    {
        b++;
    }
    if(a+b==5)
    {
        sum=sum+300000;
    }
    if(a+b==4)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((B+b+1<21&&A+b+1<21)&&Array1[A+b+1][B+b+1]==0)
                sum=sum+60000;
    }
    a=0;b=0;
    //右上、左下
    while ((B+a+1<21&&A-a-1>=0)&&Array1[A-a-1][B+a+1]==fan)
    {
        a++;
    }
    while ((B-b-1>=0&&A+b+1<21)&&Array1[A+b+1][B-b-1]==fan)
    {
        b++;
    }
    if(a+b==5)
    {
        sum=sum+300000;
    }
    if(a+b==4)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A][B-a-1]==0)
            if((B+b+1<21&&A+b+1<21)&&Array1[A][B+b+1]==0)
                sum=sum+60000;
    }
    return sum;
}


