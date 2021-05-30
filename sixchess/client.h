#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>//通信套接字


namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    //重写绘图函数
     void paintEvent(QPaintEvent *);
private:
    Ui::Client *ui;
    QTcpSocket *tcpSocket=NULL;//通信套接字

signals:
     void btnclicked();
private slots:
     void on_btn_clicked();
};

#endif // CLIENT_H
