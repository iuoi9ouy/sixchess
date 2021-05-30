#ifndef SERVICE_H
#define SERVICE_H

#include <QMainWindow>
#include <QTcpServer>//监听套接字
#include <QTcpSocket>//通信套接字
class Service : public QMainWindow
{
    Q_OBJECT
public:
    int x,y,X,Y;
    bool  X1=true;
    QTcpServer *tcpServer=NULL; //监听套接字
    QTcpSocket *tcpScoket=NULL;//通信套接字
    explicit Service(QWidget *parent = nullptr);
    //重写绘图函数
     void paintEvent(QPaintEvent *);


     //重写鼠标点击事件
     virtual void  mousePressEvent(QMouseEvent *ev);
signals:
     void btnclicked();
};

#endif // SERVICE_H
