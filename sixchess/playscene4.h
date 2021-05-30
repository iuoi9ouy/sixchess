#ifndef PLAYSCENE4_H
#define PLAYSCENE4_H

#include <QMainWindow>
#include "clientwidget.h"
#include "serverwidget.h"
class PlayScene4 : public QMainWindow
{
    Q_OBJECT
public:
    ServerWidget *server=NULL;
    ClientWidget  *client=NULL;
    explicit PlayScene4(QWidget *parent = nullptr);
    //重写MainWindow的绘图函数
     void paintEvent(QPaintEvent *);

signals:
     void btnclicked();

};

#endif // PLAYSCENE4_H
