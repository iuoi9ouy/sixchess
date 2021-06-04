 #include "playscene2.h"
#include "chess.h"
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QFont>
#include <QtGlobal>
#include <QTime>
PlayScene2::PlayScene2(QWidget *parent) : QMainWindow(parent)
{
//设置窗口
    //设置主窗口标题
    setWindowTitle("六子棋");
    //设置窗口的固定大小
    setFixedSize(800,600);
    setWindowIcon(QIcon(":/res/003.png"));
    //对数组进行赋初值为0
    array.resize(20);
    for(int i=0;i<20;i++)
    {
        array[i].resize(20);
    }
    for (int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
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


   QPushButton *btn1=new   QPushButton(this);
   //设置选择黑子按钮的大小
   btn1->setFixedSize(80,40);
   //设置选择黑子按钮上显示的内容
   btn1->setText("黑子");
   btn1->move(this->width() - btn->width()-50 , this->height() - btn->height()-300);
   //点击了黑子按钮,将pix置为1
   connect(btn1,&QPushButton::clicked,this,[=](){
      pix=1;
   });

   QPushButton *btn2=new   QPushButton(this);
   //设置选择白子按钮的大小
   btn2->setFixedSize(80,40);
   //设置选择黑子按钮上显示的内容
   btn2->setText("白子");
   btn2->move(this->width() - btn->width()-50 , this->height() - btn->height()-250);

   qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
   int t=qrand()%25;
   x=8+t/5;
   y=8+t%5;
   //点击了白子按钮,将pix置为2，同时程序自动下下一颗黑子
   connect(btn2,&QPushButton::clicked,this,[=](){
      pix=2;
      array[x][y]=1;
      QString str=":/res/002.png";
      Chess[x][y]=new chess(str);
      Chess[x][y]->setParent(this);
      Chess[x][y]->move(150+25*y-10,50+25*x-10);
      Chess[x][y]->show();
   });

}


void PlayScene2::paintEvent(QPaintEvent *)
{

//绘制棋盘
    QPainter painter(this);
    //把背景填充为褐色
    QBrush brush(QColor(210,175,142));
    painter.setBrush(brush);
    painter.drawRect(QRect(0,0,800,600));

    //绘制横线
    QPen  pen (QColor(0,0,0));
    painter.setPen(pen);
    int b=50;
    for(int i=0;i<20;i++)
    {
        painter.drawLine(QPoint(150,25*i+b),QPoint(625,25*i+b));
    }
    //绘制竖线
    b=150;
    for(int i=0;i<20;i++)
    {
        painter.drawLine(QPoint(i*25+150,50),QPoint(i*25+150,525));
    }
}



//鼠标点击事件
void PlayScene2::mousePressEvent(QMouseEvent *ev)
{
   if(pix)              //判断是否点击了黑子或者白子按钮
   {
       bool  t=false;
       if(!jieshu)
       {
           //获取鼠标点击位置的坐标
           y=(ev->x()-150+12.5)/25;
           x=(ev->y()-50+12.5)/25;
           if(((x>=0&&x<20)&&(y>=0&&y<20))&&(!array[x][y]))   //先判断鼠标点击的位置在范围内且没有下子
           {
               if(!array[x][y])            //先判断该位置有没有下了棋
               {
                   if(pix==1)              //此时下黑子
                   {
                       array[x][y]=pix;
                       QString str=":/res/002.png";
                       Chess[x][y]=new chess(str);
                       Chess[x][y]->setParent(this);
                       Chess[x][y]->move(150+25*y-10,50+25*x-10);
                       Chess[x][y]->show();
                   }
                   else              //此时下白子
                   {
                       array[x][y]=pix;
                       QString str=":/res/004.png";
                       Chess[x][y]=new chess(str);
                       Chess[x][y]->setParent(this);
                       Chess[x][y]->move(150+25*y-10,50+25*x-10);
                       Chess[x][y]->show();
                   }
               }
               //调用panduan函数，判断玩家是否胜利
               bool b=panduan();
               if(b)
               {
                   if(array[x][y]==1)   //黑子胜利
                   {
                       jieshu=true;
                       QString str="黑子胜利";
                       label= new QLabel(this);
                       label->setText(str);
                       label->setFont(QFont(str,29,QFont::Bold));
                       label ->setStyleSheet("color:red;");
                       label->setFixedSize(200,100);
                       label->move(300,250);
                       label->show();
                   }
                   else
                   {
                       jieshu=true;
                       QString str="白子胜利";
                       label= new QLabel(this);
                       label->setText(str);
                       label->setFont(QFont(str,29,QFont::Bold));
                       label ->setStyleSheet("color:red;");
                       label->setFixedSize(200,100);
                       label->move(300,250);
                       label->show();
                   }
               }
               else
                   t=true;

               //如果玩家选择了黑子在下完黑子后判断黑子是否触发禁手
               if(pix==1&&!jieshu)
               {
                   if(Panduan(x,y))
                   {
                       jieshu=true;
                       QString str="白子胜利";
                       label= new QLabel(this);
                       label->setText(str);
                       label->setFont(QFont(str,29,QFont::Bold));
                       label ->setStyleSheet("color:red;");
                       label->setFixedSize(200,100);
                       label->move(300,250);
                       label->show();
                   }
               }

               if(pix==1)           //当玩家选择黑子时，要在玩家下完黑子后判断是否和局，棋盘可下的子为奇数
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

                if(!jieshu)          //如果玩家下完已经赢了的话要禁止本次点击后程序自动下棋
                {
                    //程序第一次下白子时最好下在黑子附近
                    if(DIYI&&pix==1)
                    {
                        DIYI=false;
                        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
                        int n=qrand()%9;//随机产生0-8的数
                        while (n==4)     //避免下在黑子的位置
                        {
                            n=qrand()%9+1;
                        }
                        array[x-1+n/3][y-1+n%3]=2;
                        QString str=":/res/004.png";
                        Chess[x-1+n/3][y-1+n%3]=new chess(str);
                        Chess[x-1+n/3][y-1+n%3]->setParent(this);
                        Chess[x-1+n/3][y-1+n%3]->move(150+25*(y-1+n%3)-10,50+25*(x-1+n/3)-10);
                        Chess[x-1+n/3][y-1+n%3]->show();
                        qDebug()<<x-1+n/3<<y-1+n%3;
                    }
                    else
                    {
                        //程序自动下棋
                        if(pix==1)
                        {
                           int u=xiaqi();
                           x=u/100;
                           y=u%100;
                        }
                        if(pix==2)
                        {
                            int u=xiaqi();
                            x=u/100;
                            y=u%100;
                        }
                        if(pix==1)
                        {
                            array[x][y]=2;
                            QString str=":/res/004.png";
                            Chess[x][y]=new chess(str);
                            Chess[x][y]->setParent(this);
                            Chess[x][y]->move(150+25*y-10,50+25*x-10);
                            Chess[x][y]->show();
                        }

                        if(pix==2)
                        {
                            array[x][y]=1;
                            QString str=":/res/002.png";
                            Chess[x][y]=new chess(str);
                            Chess[x][y]->setParent(this);
                            Chess[x][y]->move(150+25*y-10,50+25*x-10);
                            Chess[x][y]->show();
                        }

                      //调用panduan函数，判断程序是否胜利
                       b=panduan();

                      if(b&&t)
                      {
                          if(pix==1)   //白子胜利
                          {
                              jieshu=true;
                              QString str="白子胜利";
                              qDebug()<<str;
                              label= new QLabel(this);
                              label->setText(str);
                              label->setFont(QFont(str,29,QFont::Bold));
                              label ->setStyleSheet("color:red;");
                              label->setFixedSize(200,100);
                              label->move(300,250);
                              label->show();
                          }
                          else
                          {
                              jieshu=true;
                              QString str="黑子胜利";
                              label= new QLabel(this);
                              label->setText(str);
                              label->setFont(QFont(str,29,QFont::Bold));
                              label ->setStyleSheet("color:red;");
                              label->setFixedSize(200,100);
                              label->move(300,250);
                              label->show();
                          }
                      }

                      //如果玩家选择了白子则要在程序下完黑子后判断一下是否和局
                      if(pix==2)
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
}


//判断是否胜利
bool  PlayScene2::panduan()
{
    bool t=false;//默认为未胜利，为true时胜利
    int  k=0;  //k用来存储遍历时该格的数据
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
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
            while (j+b+1<20&&array[i][j+b+1]==k)
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
            while (i+b+1<20&&array[i+b+1][j]==k)
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
           while ((i+b+1<20&&j+b+1<20)&&array[i+b+1][j+b+1]==k)
           {
               b++;
           }
           if(a+b==5)
           {
               t=true;
               return t;
           }

           a=0;b=0;
          while((i-a-1>=0&&j+a+1<20)&&array[i-a-1][j+a+1]==k)     //对左下方向进行判断
          {
              a++;
          }
          while ((i+b+1<20&&j-b-1>=0)&&array[i+b+1][j-b-1]==k)
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
bool  PlayScene2::Panduan(int A,int B)
{
//先判断五五禁手
    int a=0,b=0;
    int T[4]={0,0,0,0};         //用来判断4条线上有无符合条件的  符合则置1   T[0]竖直  T[1]水平 T[2]右下斜对角线 T[3]左下斜对角线
    //判断竖直方向
    while(B-a-1>=0&&array[A][B-a-1]==1)
    {
        a++;
    }
    while (B+b+1<20&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if((B-a-1>=0&&B+b+1<20)&&(array[A][B-a-1]==0||array[A][B+b+1]==0))
                T[0]=1;
    }
    if(a+b==3)
    {
        if(B-a-2>=0)
        {
            if(array[A][B-a-1]==0&&array[A][B-a-2]==1)
                T[0]=1;
        }
        if(B+b+2<20)
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
        if(B+b+5<20)
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
        if(B+b+3<20)
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
    while (A+b+1<20&&array[A+b+1][B]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if((A-a-1>=0&&A+b+1<20)&&(array[A-a-1][B]==0||array[A+b+1][B]==0))
                T[1]=1;
    }
    if(a+b==3)
    {
        if(A-a-2>=0)
        {
            if(array[A-a-1][B]==0&&array[A-a-2][B]==1)
                T[1]=1;
        }
        if(A+b+2<20)
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
        if(A+b+5<20)
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
        if(A+b+3<20)
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
    while ((A+b+1<20&&B+b+1<20)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if(((A-a-1>=0&&B-a-1>=0)&&(A+b+1<20&&B+b+1<20))&&(array[A-a-1][B-a-1]==0||array[A+b+1][B+b+1]==0))
                T[2]=1;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B-a-2>=0)
        {
            if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==1)
                T[2]=1;
        }
        if(A+b+2<20&&B+b+2<20)
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
        if(A+b+5<20&&B+b+5<20)
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
        if(A+b+3<20&&B+b+3<20)
            if(array[A+b+1][B+b+1]==0&&array[A+b+2][B+b+2]==1&&array[A+b+3][B+b+3]==1)
            {
               T[2]=1;
            }
    }
    a=0;b=0;
    //判断左下斜对角线方向
    while((A- a-1>=0&&B+a+1<20)&&array[A-a-1][B+a+1]==1)
    {
        a++;
    }
    while ((A+b+1<20&&B-b-1>=0)&&array[A+b+1][B-+b-1]==1)
    {
        b++;
    }
    if(a+b==4)
    {
        if(((A- a-1>=0&&B+a+1<20)&&(A+b+1<20&&B-b-1>=0))&&(array[A-a-1][B+a+1]==0||array[A+b+1][B-+b-1]==0))
                T[3]=1;
    }

    if(a+b==3)
    {
        if(A-a-2>=0&&B+a+2<20)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1)
                T[3]=1;
        }
        if(A+b+2<20&&B-b-2>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1)
                T[3]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0&&B+a+5<20)
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==1&&array[A-a-5][B+a+5]==1)
            {
                T[3]=1;
            }
        if(A+b+5<20&&B-b-5>=0)
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==1&&array[A+b+5][B-b-5]==1)
            {
                T[3]=1;
            }
    }
    if(a+b==1)
    {
        if(A-a-3>=0&&B+a+3<20)
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1)
            {
                T[3]=1;
            }
        if(A+b+3<20&&B-b-3>=0)
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
    while (B+b+1<20&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(B-a-2>=0)
            if(B+b+1<20)
                if(array[A][B-a-1]==0&&array[A][B-a-2]==0&&array[A][B+b+1]==0)
                    T[0]=1;
        if(B-a-1>=0)
            if(B+b+2<20)
                if(array[A][B-a-1]==0&&array[A][B+b+1]==0&&array[A][B+b+2]==0)
                    T[0]=1;
    }
    if(a+b==2)
    {
        if(B-a-3>=0)
        {
            if((array[A][B-a-1]==0)&&(array[A][B-a-2]==1)&&(array[A][B-a-3]==0))
            {
              if(B+b+1<20&&array[A][B+b+1]==0)
                  T[0]=1;
            }

        }
        if(B-a-1>=0&&array[A][B-a-1]==0)
        {
          if(B+b+3<20)
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
                if(B+b+1<20&&array[A][B+b+1]==0)
                    T[0]=1;
        }

        if(B+b+4<20)
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
                if(B+b+1<20&&array[A][B+b+1]==0)
                    T[0]=1;
            }
        }
        if(B+b+5<20)
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
    while (A+b+1<20&&array[A+b+1][B]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0)
            if(A+b+1<20)
                if(array[A-a-1][B]==0&&array[A-a-2][B]==0&&array[A+b+1][B]==0)
                    T[1]=1;
        if(A-a-1>=0)
            if(A+b+2<20)
                if(array[A-a-1][B]==0&&array[A+b+1][B]==0&&array[A+b+2][B]==0)
                    T[1]=1;
    }
    if(a+b==2)
    {
        if(A-a-3>=0)
        {
            if((array[A-a-1][B]==0)&&(array[A-a-2][B]==1)&&(array[A-a-3][B]==0))
            {
              if(A+b+1<20&&array[A+b+1][B]==0)
                  T[1]=1;
            }
        }

        if(A-a-1>=0&&array[A-a-1][B]==0)
        {
          if(A+b+3<20)
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
                if(A+b+1<20&&array[A+b+1][B]==0)
                    T[1]=1;
        }
        if(A+b+4<20)
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
                if(A+b+1<20&&array[A+b+1][B]==0)
                    T[1]=1;
            }
        }
        if(A+b+5<20)
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
    while ((A+b+1<20&&B+b+1<20)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B-a-2>=0)
            if(A+b+1<20&&B+b+1<20)
                if(array[A-a-1][B-a-1]==0&&array[A-a-2][B-a-2]==0&&array[A+b+1][B+b+1]==0)
                    T[2]=1;
        if(A-a-1>=0&&B-a-1>=0)
            if(A+b+2<20&&B+b+2<20)
                if(array[A-a-1][B-a-1]==0&&array[A+b+1][B+b+1]==0&&array[A+b+2][A+b+2]==0)
                    T[2]=1;
    }
    if(a+b==2)
    {
        if(B-a-3>=0&&A-a-3>=0)
        {
            if((array[A-a-1][B-a-1]==0)&&(array[A-a-2][B-a-2]==1)&&(array[A-a-3][B-a-3]==0))
            {
              if((B+b+1<20&&A+b+1<20)&&array[A+b+1][B+b+1]==0)
                  T[2]=1;
            }
        }
        if((B-a-1>=0&&A-a-1>=0)&&array[A-a-1][B-a-1]==0)
        {
          if(B+b+3<20&&A+b+3<20)
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
                if((B+b+1<20&&A+b+1<20)&&array[A+b+1][B+b+1]==0)
                    T[2]=1;

        }
        if(A+b+4<20&&B+b+4<20)
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
                if((B+b+1<20&&A+b+1<20)&&array[A+b+1][B+b+1]==0)
                    T[2]=1;
            }
        }
        if(B+b+5<20&&A+b+5<20)
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
    while((A-a-1>=0&&B+a+1<20)&&array[A-a-1][B+a+1]==1)
    {
        a++;
    }
    while ((A+b+1<20&&B-b-1>=0)&&array[A+b+1][B-+b-1]==1)
    {
        b++;
    }
    if(a+b==3)
    {
        if(A-a-2>=0&&B+a+2<20)
            if(A+b+1<20&&B-b-1>=0)
                if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==0&&array[A+b+1][B-b-1]==0)
                    T[3]=1;
        if(A-a-1>=0&&B+a+1<20)
            if(A+b+2<20&&B-b-2>=0)
                if(array[A-a-1][B+a+1]==0&&array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==0)
                    T[3]=1;
    }
    if(a+b==2)
    {
        if(B+a+3<20&&A-a-3>=0)
        {
            if((array[A-a-1][B+a+1]==0)&&(array[A-a-2][B+a+2]==1)&&(array[A-a-3][B+a+3]==0))
            {
              if((A+b+1<20&&B-b-1>=0)&&array[A+b+1][B-b-1]==0)
                  T[3]=1;
            }
        }
        if((B+a+1<20&&A-a-1>=0)&&array[A-a-1][B+a+1]==0)
        {
          if(A+b+3<20&&B-b-3>=0)
          {
              if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==0)
                  T[3]=1;
          }
        }
    }
    if(a+b==1)
    {
        if(B+a+4<20&&A-a-4>=0)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==0)
                if((B-b-1<20&&A+b+1>=0)&&array[A+b+1][B-b-1]==0)
                    T[0]=1;

        }
        if(A+b+4<20&&B-b-4>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==0)
                if((A-a-1>=0&&B+a+1<20)&&array[A-a-1][B+a+1]==0)
                    T[0]=1;
        }
    }
    if(a+b==0)
    {
        if(A-a-5>=0&&B+a+5<20)
        {
            if(array[A-a-1][B+a+1]==0&&array[A-a-2][B+a+2]==1&&array[A-a-3][B+a+3]==1&&array[A-a-4][B+a+4]==1&&array[A-a-5][B+a+5]==0)
            {
                if((A+b+1<20&&B-b-1>=0)&&array[A+b+1][B-b-1]==0)
                    T[1]=1;
            }
        }
        if(A+b+5<20&&B-b-5>=0)
        {
            if(array[A+b+1][B-b-1]==0&&array[A+b+2][B-b-2]==1&&array[A+b+3][B-b-3]==1&&array[A+b+4][B-b-4]==1&&array[A+b+5][B-b-5]==0)
            {
                if((A-a-1>=0&&B+a+1<20)&&array[A-a-1][B+a+1]==0)
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
    while (B+b+1<20&&array[A][B+b+1]==1)
    {
        b++;
    }
    if(a+b>5){T[0]=2;}

     a=0;b=0;
    while(A-a-1>=0&&array[A-a-1][B]==1)     //对竖直方向进行判断
    {
        a++;
    }
    while (A+b+1<20&&array[A+b+1][B]==1)
    {
        b++;
    }
     if(a+b>5){T[0]=2;}

    a=0;b=0;
    while((A-a-1>=0&&B-a-1>=0)&&array[A-a-1][B-a-1]==1)     //对右下方向进行判断
    {
        a++;
    }
    while ((A+b+1<20&&B+b+1<20)&&array[A+b+1][B+b+1]==1)
    {
        b++;
    }
    if(a+b>5){T[0]=2;}

    a=0;b=0;
    while((A-a-1>=0&&B+a+1<20)&&array[A-a-1][B+a+1]==1)     //对左下方向进行判断
    {
       a++;
    }
    while ((A+b+1<20&&B-b-1>=0)&&array[A+b+1][B-b-1]==1)
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

int   PlayScene2::pingfen(int A,int B,int C)
{
    int a=0;int b=0;
    int sum=0;              //用于统计分数
    int Array1[20][20];     //用于复刻棋盘上的棋子
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
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
    while (B+b+1<20&&Array1[A][B+b+1]==C)
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
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+50000;
        else
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //眠五
                sum=sum+50000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //活四
                sum=sum+4000;
            else                                            //眠四
                sum=sum+250;
        else
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //活三
                sum=sum+3000;
            else                                            //眠三
                sum=sum+150;
        else
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+20;
        else
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if(B-a-1>=0&&Array1[A][B-a-1]==0)
            if(B+b+1<20&&Array1[A][B+b+1]==0)                //活单
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
    while(A+b+1<20&&Array1[A+b+1][B]==C)
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
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+50000;
        else
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //眠五
                sum=sum+50000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //活四
                sum=sum+4000;
            else                                            //眠四
                sum=sum+2500;
        else
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //活三
                sum=sum+3000;
            else                                            //眠三
                sum=sum+1500;
        else
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if(A-a-1>=0&&Array1[A-a-1][B]==0)
            if(A+b+1<20&&Array1[A+b+1][B]==0)                //活单
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
    while ((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==C)
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
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)               //活五
                sum=sum+60000;
            else                                            //眠五
                sum=sum+50000;
        else
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)                 //眠五
                sum=sum+50000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)                 //活四
                sum=sum+4000;
            else                                            //眠四
                sum=sum+2500;
        else
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)                 //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)            //活三
                sum=sum+3000;
            else                                            //眠三
                sum=sum+1500;
        else
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)                 //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)              //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)             //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if((A-a-1>=0&&B-a-1>=0)&&Array1[A-a-1][B-a-1]==0)
            if((A+b+1<20&&B+b+1<20)&&Array1[A+b+1][B+b+1]==0)              //活单
                sum=sum+20;
            else
                sum=sum+10;
        else
                sum=sum+10;
    }
    a=0;b=0;
    //判断左下斜对角线方向
    while((A- a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==C)
    {
        a++;
    }
    while ((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-+b-1]==C)
    {
        b++;
    }
    if(a+b==5)                  //连成六子
    {
        sum=sum+10000000;
    }
    if(a+b==4)                  //连成五子
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)               //活五
                sum=sum+6000;
            else                                            //眠五
                sum=sum+5000;
        else
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)                 //眠五
                sum=sum+5000;
            else
                sum=sum+10;                                  //死五
    }
    if(a+b==3)                  //连成四子
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)          //活四
                sum=sum+4000;
            else                                            //眠四
                sum=sum+2500;
        else
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)                 //眠四
                sum=sum+2500;
            else
                sum=sum+10;                                  //死四
    }
    if(a+b==2)                  //连成三子
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)          //活三
                sum=sum+3000;
            else                                            //眠三
                sum=sum+1500;
        else
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)             //眠三
                sum=sum+1500;
            else
                sum=sum+10;                                  //死三
    }
    if(a+b==1)                  //连成二子
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)             //活二
                sum=sum+1000;
            else                                            //眠二
                sum=sum+200;
        else
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)           //眠二
                sum=sum+200;
            else
                sum=sum+10;                                  //死二
    }
    if(a+b==0)
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            if((A+b+1<20&&B-b-1>=0)&&Array1[A+b+1][B-b-1]==0)              //活单
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
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
        if(B-a-1>=0&&Array1[A][ B-a-1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
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
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
        if((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][ B-a-1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
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
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
       if(A-a-1>=0&&Array1[A-a-1][ B]==0)
            sum=sum+2000;
        else
            sum=sum+500;
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
    while ((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
       if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
    }
    if(a==2)
    {
        if((A-a-1>=0&&B+a+1<20)&&Array1[A-a-1][B+a+1]==0)
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
    while (B+a+1<20&&Array1[A][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(B+a+1<20&&Array1[A][ B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if(B+a+1<20&&Array1[A][ B+a+1]==0)
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
       if(B+a+1<20&&Array1[A][ B+a+1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
    }
    if(a==2)
    {
         if(B+a+1<20&&Array1[A][ B+a+1]==0)
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
    while ((B+a+1<20&&A+a+1<20)&&Array1[A+a+1][B+a+1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((B+a+1<20&&A+a+1<20)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if((B+a+1<20&&A+a+1<20)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
        if((B+a+1<20&&A+a+1<20)&&Array1[A+a+1][ B+a+1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
    }
    if(a==2)
    {
         if((B+a+1<20&&A+a+1<20)&&Array1[A+a+1][ B+a+1]==0)
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
    while (A+a+1<20&&Array1[A+a+1][B]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if(A+a+1<20&&Array1[A+a+1][ B]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
       if(A+a+1<20&&Array1[A+a+1][ B]==0)
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
        if(A+a+1<20&&Array1[A+a+1][ B]==0)
            sum=sum+2000;
        else
            sum=sum+500;
    }
    if(a==2)
    {
         if(A+a+1<20&&Array1[A+a+1][ B]==0)
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
    while ((B-a-1>=0&&A+a+1<20)&&Array1[A+a+1][B-a-1]==fan)
    {
        a++;
    }
    if(a==5)
    {
        if((B-a-1>=0&&A+a+1<20)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+300000;
        else
            sum=sum+400000;
    }
    if(a==4)
    {
      if((B-a-1>=0&&A+a+1<20)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+20000;
        else
            sum=sum+1500;
    }
    if(a==3)
    {
        if((B-a-1>=0&&A+a+1<20)&&Array1[A+a+1][ B-a-1]==0)
            sum=sum+2000;
        else
            sum=sum+500;
    }
    if(a==2)
    {
         if((B-a-1>=0&&A+a+1<20)&&Array1[A+a+1][ B-a-1]==0)
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
    while (A+b+1<20&&Array1[A+b+1][B]==fan)
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
            if(A+b+1<20&&Array1[A+b+1][B]==0)
                sum=sum+20000;
    }
    a=0;b=0;
    //左右方向
    while (B-a-1>=0&&Array1[A][B-a-1]==fan)
    {
        a++;
    }
    while (B+b+1<20&&Array1[A][B+b+1]==fan)
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
            if(B+b+1<20&&Array1[A][B+b+1]==0)
                sum=sum+20000;
    }

    a=0;b=0;
    //左上、右下
    while ((B-a-1>=0&&A-a-1>=0)&&Array1[A-a-1][B-a-1]==fan)
    {
        a++;
    }
    while ((B+b+1<20&&A+b+1<20)&&Array1[A+b+1][B+b+1]==fan)
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
            if((B+b+1<20&&A+b+1<20)&&Array1[A+b+1][B+b+1]==0)
                sum=sum+20000;
    }
    a=0;b=0;
    //右上、左下
    while ((B+a+1<20&&A-a-1>=0)&&Array1[A-a-1][B+a+1]==fan)
    {
        a++;
    }
    while ((B-b-1>=0&&A+b+1<20)&&Array1[A+b+1][B-b-1]==fan)
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
            if((B+b+1<20&&A+b+1<20)&&Array1[A][B+b+1]==0)
                sum=sum+20000;
    }
    return sum;
}


int PlayScene2::xiaqi()
 {
     int r=0;
     int k=0;               //k为黑白子
     if(pix==1)
         k=2;
     if(pix==2)
         k=1;
     int a[20][20];
     int t=0;
     //将数组a里面的值全部改为-10
     for(int i=0;i<20;i++)
     {
         for(int j=0;j<20;j++)
         {
          a[i][j]=0;
         }
     }
     //如果数组array 里面为0，则说明该位置还没有下此时在数组a存储评分   把已经下了的排除在外
     for(int i=0;i<20;i++)
     {
         for(int j=0;j<20;j++)
         {
             if(array[i][j]==0)
                  a[i][j]=pingfen(i,j,k);
         }
     }
     //取出数组a里面评分最大的值
     for(int i=0;i<20;i++)
     {
         for(int j=0;j<20;j++)
         {
             if(a[i][j]>t)
                 t=a[i][j];
         }
     }
    bool  l=false;
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
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




