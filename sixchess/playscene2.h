#ifndef PLAYSCENE2_H
#define PLAYSCENE2_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QVector>
#include <QLabel>
#include <chess.h>

class PlayScene2 : public QMainWindow
{
    Q_OBJECT
public:
    bool  DIYI=true;
    bool   jieshu=false;//jieshu 用于判断该局是否结束了
    int x,y;//用于存贮下的棋子的位置
    chess  *Chess[20][20];//棋子指针
    QLabel *label;//用于显示胜利
    //pix用于记录玩家选择的棋子 1为黑 2为白   0为还未选择
    int pix=0;
    //定义一个二维数组,用于存储棋盘上的情况 0为空 1为黑 2为白
    QVector<QVector<int>> array;

    explicit PlayScene2(QWidget *parent = nullptr);

    //重写鼠标点击事件
    virtual void  mousePressEvent(QMouseEvent *ev);
    //重写绘图事件
     void paintEvent(QPaintEvent *);
     //判断是否胜利
     bool  panduan();
     //判断黑子是否触发了禁手
     bool  Panduan(int A,int B);
     //统计某个空位上的分数   A 为x  B为 y ，C 为假设该格为黑白子  1为黑  2为白
     int   pingfen(int A,int B,int C);
     //程序自动下棋   k的值为黑、白棋
     int  xiaqi();
signals:
     void btnclicked();
};

#endif // PLAYSCENE2_H
