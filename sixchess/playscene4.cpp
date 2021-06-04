#include "playscene4.h"
#include <QPushButton>
#include <QPainter>


PlayScene4::PlayScene4(QWidget *parent) : QMainWindow(parent)
{

//设置窗口
      //设置主窗口标题
      setWindowTitle("六子棋");
      //设置窗口的固定大小
      setFixedSize(500,400);
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


      //设置client(客户端)按钮
      QPushButton *btn1 = new QPushButton;
      btn1->setParent(this);
      //设置client按钮的大小
      btn1->setFixedSize(80,40);
      //设置client按钮上显示的内容
      btn1->setText("client");
      //移动client按钮
      btn1->move(80,320);

      connect(btn1,&QPushButton::clicked,this,[=](){
         client= new ClientWidget;
         client->show();
         this->hide();
         connect( client,&ClientWidget::btnclicked,this,[=](){
             this->show(); //重新显示主场景
              client->hide();
             delete  client; //将下棋场景 关闭掉
              client=NULL;
         });
      });



      //设置service(服务器)按钮
      QPushButton *btn2 = new QPushButton;
      btn2->setParent(this);
      //设置service按钮的大小
      btn2->setFixedSize(80,40);
      //设置service按钮上显示的内容
      btn2->setText("service");
      //移动service按钮
      btn2->move(260,320);

      connect(btn2,&QPushButton::clicked,this,[=](){
         server= new ServerWidget;
         server->show();
         this->hide();
         connect( server,&ServerWidget::btnclicked,this,[=](){
             this->show(); //重新显示主场景
            server->hide();
             delete   server; //将下棋场景 关闭掉
              server=NULL;
         });
      });




}





void PlayScene4::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;

    //画背景上图标
    pix.load(":/res/001.png");

    pix = pix.scaled( pix.width(), pix.height());

    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
