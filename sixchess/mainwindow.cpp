#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playscene.h"
#include <QPainter>
#include <QIcon>
#include <QDebug>
#include <QPushButton>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置主窗口标题
    setWindowTitle("六子棋");
    //设置窗口的固定大小
    setFixedSize(800,600);
    setWindowIcon(QIcon(":/res/003.png"));

   //创建双人对弈按钮
   QPushButton *btn1 = new QPushButton;
   btn1->setParent(this);
   //设置返回按钮的大小
   btn1->setFixedSize(80,40);
   //设置返回按钮上显示的内容
   btn1->setText("双人对弈");
   //移动双人对弈按钮
   btn1->move(50,500);


//    PlayScene *playscene=new PlayScene;
    connect(btn1,&QPushButton::clicked,[=](){
        playscene1 =new PlayScene1;
        //接收游戏界面发出的返回信号
         connect(playscene1,&PlayScene1::btnclicked,this,[=](){
//             qDebug()<<"接收";
             this->show(); //重新显示主场景
             playscene1->hide();
             delete playscene1; //将下棋场景 关闭掉
             playscene1=NULL;
         });
        //显示游戏界面
        playscene1->show();
        //隐藏主界面
        this->hide();
    });

     //创建人机对弈按钮
     QPushButton *btn2 = new QPushButton;
     btn2->setParent(this);
     //设置返回按钮的大小
     btn2->setFixedSize(80,40);
     //设置返回按钮上显示的内容
     btn2->setText("人机对弈");
     //移动双人对弈按钮
     btn2->move(250,500);



     connect(btn2,&QPushButton::clicked,[=](){
         playscene2 =new PlayScene2;
         //接收游戏界面发出的返回信号
          connect(playscene2,&PlayScene2::btnclicked,this,[=](){
 //             qDebug()<<"接收";
              this->show(); //重新显示主场景
              playscene2->hide();
              delete playscene2; //将下棋场景 关闭掉
              playscene2=NULL;
          });
         //显示游戏界面
         playscene2->show();
         //隐藏主界面
         this->hide();
     });


     QPushButton *btn3 = new QPushButton;
     btn3->setParent(this);
     //设置返回按钮的大小
     btn3->setFixedSize(80,40);
     //设置返回按钮上显示的内容
     btn3->setText("机机对弈");
     //移动双人对弈按钮
     btn3->move(450,500);
     connect(btn3,&QPushButton::clicked,[=](){
         playscene3 =new PlayScene3;
         //接收游戏界面发出的返回信号
          connect(playscene3,&PlayScene3::btnclicked,this,[=](){
 //             qDebug()<<"接收";
              this->show(); //重新显示主场景
              playscene3->hide();
              delete playscene3; //将下棋场景 关闭掉
              playscene3=NULL;
          });
         //显示游戏界面
         playscene3->show();
         //隐藏主界面
         this->hide();
     });

     //创建联机对弈按钮
     QPushButton *btn4 = new QPushButton;
     btn4->setParent(this);
     //设置返回按钮的大小
     btn4->setFixedSize(80,40);
     //设置返回按钮上显示的内容
     btn4->setText("联机对抗");
     //移动双人对弈按钮
     btn4->move(650,500);
     connect(btn4,&QPushButton::clicked,[=](){
         playscene4 =new PlayScene4;
         //接收游戏界面发出的返回信号
          connect(playscene4,&PlayScene4::btnclicked,this,[=](){
 //             qDebug()<<"接收";
              this->show(); //重新显示主场景
              playscene4->hide();
              delete playscene4; //将下棋场景 关闭掉
              playscene4=NULL;
          });
         //显示游戏界面
         playscene4->show();
         //隐藏主界面
         this->hide();
     });
     QPushButton *btn8 = new QPushButton;
     btn8->setParent(this);
     //设置关闭按钮的大小
     btn8->setFixedSize(60,40);
     //设置关闭按钮上显示的内容
     btn8->setText("关闭");
     //移动关闭对弈按钮
     btn8->move(this->width()-btn8->width(),this->height()-btn8->height());
     connect(btn8,&QPushButton::clicked,[=](){
         this->close();
     });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;

    //画背景上图标
    pix.load(":/res/003.png");

    pix = pix.scaled( pix.width(), pix.height());

    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}


