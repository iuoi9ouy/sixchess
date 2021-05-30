#ifndef PLAYSCENE3_H
#define PLAYSCENE3_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QVector>
#include <QLabel>
#include <chess.h>
#include <QTimerEvent>
#include <QTimer>

class PlayScene3 : public QMainWindow
{
    Q_OBJECT
public:
    int x,y;
    //定义一个二维数组,用于存储棋盘上的情况 0为空 1为黑 2为白
    QVector<QVector<int>> array;
    chess  *Chess[20][20];//棋子指针
    bool  kaishi=false;   //设置开始值，在未点击开始按钮时为false  点击后为true   棋局结束后应该为false
    int pix=1;      //刚开始时把黑子置为1
    bool  baizi=true;//第一次下白子
    bool  heizi=true;//第一次下黑子
    QLabel *label;//用于显示胜利
    explicit PlayScene3(QWidget *parent = nullptr);
    //重写绘图事件
     void paintEvent(QPaintEvent *);
     //判断是否胜利
     bool  panduan();
     //统计某个空位上的分数   A 为x  B为 y ，C 为假设该格为黑白子  1为黑  2为白
     int   pingfen(int A,int B,int C);
     //程序自动下棋   k的值为黑、白棋
     int  xiaqi();
     //重写计时器事件
     void  timerEvent(QTimerEvent *);
     //判断是否禁手
     bool  Panduan(int A,int B);

signals:
    void btnclicked();
};

#endif // PLAYSCENE3_H
