#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>//监听套接字
#include <QTcpSocket>//通信套接字
#include "chess.h"
#include <QMouseEvent>
#include <QVector>
#include <QLabel>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    bool  hei=false,bai=false;//用于下第一次的黑白子,使其不是程序自动下
    bool xuanze=false;//用于记录该界面有没有选择手动或者自动
    QTimer *timer;//用于自动下棋
    bool shoudong=false,zidong=false;  //用于选择手动下棋或者自动下棋
    chess  *Chess[21][21];//棋子指针
    int array[21][21];
    bool heizi=false,baizi=false;//用于判断此时是否该下此子
    ServerWidget(QWidget *parent = nullptr);
    bool   jieshu=false;//jieshu 用于判断该局是否结束了
    int X,Y;//用于对方下的棋子的位置
    int x,y;//用于存贮下的棋子的位置
    int T5=0;
    QLabel *label;//用于显示胜利
    //pix用于判断黑白棋 1为黑 2为白,黑棋先手
    int pix=0;
    //定义一个二维数组,用于存储棋盘上的情况 0为空 1为黑 2为白

    //判断是否胜利
    bool  panduan();
    //判断黑子是否触发了禁手
    bool  Panduan(int A,int B);

    //重写鼠标点击事件
    virtual void  mousePressEvent(QMouseEvent *ev);
    //重写绘图事件
     void paintEvent(QPaintEvent *);
    ~ServerWidget();
    void  xiaqi();
    int XIAQI();                    //用于程序自动下棋，返回值为10*x+y
    int pingfen(int A,int B,int C); //用于给每个空格打分   输入 x，y，以及黑白子
    void Zidong();//用于自动下棋
private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServer=NULL;//通信套接字
    QTcpSocket *tcpSocket=NULL;//监听套接字
//public slots:
//    void  xiaqi();
//    void  lianjie();
signals:
    void btnclicked();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};
#endif // SERVERWIDGET_H
