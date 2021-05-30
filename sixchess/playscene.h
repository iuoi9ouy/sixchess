#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QVector>
#include <QLabel>
class PlayScene1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayScene1(QWidget *parent = nullptr);
    bool   jieshu=false;//jieshu 用于判断该局是否结束了
    int x,y;//用于存贮下的棋子的位置
    QLabel *label;//用于显示胜利
    //pix用于判断黑白棋 1为黑 2为白,黑棋先手
    int pix=1;
    //定义一个二维数组,用于存储棋盘上的情况 0为空 1为黑 2为白
    QVector<QVector<int>> array;



    //重写鼠标点击事件
    virtual void  mousePressEvent(QMouseEvent *ev);
    //重写绘图事件
     void paintEvent(QPaintEvent *);
     //判断是否胜利
     bool  panduan();
     //判断黑子是否触发了禁手
     bool  Panduan(int A,int B);
signals:
    void btnclicked();

};

#endif // PLAYSCENE_H
