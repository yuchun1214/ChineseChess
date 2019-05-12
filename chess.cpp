#include "chess.h"


Chess::Chess(QObject *parent) : QObject(parent)
{

}

Chess::Chess(QString name, QPoint pos, QVector<int> x, QVector<int> y, QVector<QPoint> movingVector,QVector<QPoint> addingVector,QString resPath,char flag, bool surpass, QObject *parent)
{
    this->name = name;
    this->relativePos = pos;
    this->borderX = x;
    this->borderY = y;
    this->movingVector = movingVector;
    this->addingVector = addingVector;
    this->surpass = surpass;
    this->flag = flag;
    QPixmap bgPixmap(resPath);
    this->setPixmap(bgPixmap);
   setScale(0.08);
    this->setPos(QPoint(relativePos.x() - CHESS_SIZE / 2, relativePos.y() - CHESS_SIZE / 2));
//    this->setRect(this->startPos.x() - CHESS_SIZE / 2, this->startPos.y() - CHESS_SIZE / 2, CHESS_SIZE, CHESS_SIZE);
}


void Chess::TestSlots(){
    qDebug()<<"Signal receive";
}

void Chess::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"chess press event";
    emit BeClicked(this);
}

void Chess::Move(QPoint selectedPos){
    this->relativePos = selectedPos;
    this->setX(selectedPos.x() - CHESS_SIZE / 2);
    this->setY(selectedPos.y() - CHESS_SIZE / 2);
}

QString Chess::Name(){
    return  name;
}

QVector<QPoint> Chess::MovingVector(){
    return movingVector;
}

QVector<int> Chess::BorderX(){
    return borderX;
}

QVector<int> Chess::BorderY(){
    return  borderY;
}

void Chess::SET_GLOBAL_AREA_BORDER()
{
    borderX.clear();
    borderY.clear();
    borderX.append(0);
    borderX.append(8 * BLOCK_SIZE);
    borderY.append(0);
    borderY.append(9 * BLOCK_SIZE);
}

QPoint Chess::RelativePos(){
    return  relativePos;
}

char Chess::Flag(){
    return flag;
}

void Chess::addingAddingVector()
{

    for(int i = 0, size = addingVector.size(); i < size; i++){
        movingVector.append(addingVector[i]);
    }
    addingVector.clear();
    SET_GLOBAL_AREA_BORDER();
    surpass = false;
}

bool Chess::Surpass(){
    return surpass;
}


