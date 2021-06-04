#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H
#include "chess.h"
#include <QWidget>
#include <QTcpSocket>//通信套接字
#include <QMouseEvent>
#include <QVector>
#include <QLabel>
namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT


public:
    QTimer *timer;
    bool bai=false,hei=false;//用于第一次下黑白子
    bool shoudong=false,zidong=false;  //用于选择手动下棋或者自动下棋
    chess  *Chess[21][21];//棋子指针
    bool  duimian=false;//用于判断对面有没有选择手动或者自动，在该值为true时才可以选择黑白子
    bool  k=false;//用于存储有没有进行链接
    bool   heizi=false,baizi=false;//用于判断此时是否该下此子
    bool   jieshu=false;//jieshu 用于判断该局是否结束了
    int x,y;//用于存贮下的棋子的位置
    int X,Y;//用于对方下的棋子的位置
    QLabel *label;//用于显示胜利
    //pix用于判断黑白棋 1为黑 2为白,黑棋先手
    int pix=0;
    //定义一个二维数组,用于存储棋盘上的情况 0为空 1为黑 2为白
    int array[21][21];

    //判断是否胜利
    bool  panduan();
    //判断黑子是否触发了禁手
    bool  Panduan(int A,int B);

    void xiaqi();
    //重写鼠标点击事件
    virtual void  mousePressEvent(QMouseEvent *ev);

    int XIAQI();                    //用于程序自动下棋，返回值为10*x+y
    int pingfen(int A,int B,int C); //用于给每个空格打分   输入 x，y，以及黑白子
    void Zidong();//自动下棋函数

    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();
    //重写绘图函数
     void paintEvent(QPaintEvent *);
private slots:
    void on_ButtonConnect_clicked();

    void on_Buttonhei_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ClientWidget *ui;

    QTcpSocket  *tcpSocket=NULL;//通信套接字

signals:
    void btnclicked();
};

#endif // CLIENTWIDGET_H
