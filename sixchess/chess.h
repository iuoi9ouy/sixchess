#ifndef CHESS_H
#define CHESS_H

#include <QPushButton>

class chess : public QPushButton
{
    Q_OBJECT
public:
    //构造函数   参数为黑棋或者白棋
     chess(QString Img);
     QString  ImgPath;
signals:

};

#endif // CHESS_H
