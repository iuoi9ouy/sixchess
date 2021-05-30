#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "playscene.h"
#include "playscene2.h"
#include "playscene3.h"
#include "playscene4.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //重写MainWindow的绘图函数
     void paintEvent(QPaintEvent *);
     //定义一个游戏界面的指针
    PlayScene1 *playscene1=NULL;
    PlayScene2 *playscene2=NULL;
    PlayScene3 *playscene3=NULL;
    PlayScene4 *playscene4=NULL;
private slots:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
