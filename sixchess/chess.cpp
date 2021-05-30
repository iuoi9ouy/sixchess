#include "chess.h"
#include <QPainter>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <QLabel>
#include <QTextBlock>

chess::chess(QString Img)
{
    this->ImgPath=Img;
    QPixmap pix;
    bool ret = pix.load(Img);
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }
    //设置图片固定大小
    this->setFixedSize( 20,20);
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(20,20));

}





