#include "path.h"

Path::Path(QObject *parent) : QObject(parent)
{

}


Path::Path(QPoint path_pos, Chess * selectedChess, QObject * parent) : QObject (parent){
    this->pathPos = path_pos;
    this->setRect(path_pos.x() - PATH_SIZE / 2, path_pos.y() - PATH_SIZE / 2, PATH_SIZE, PATH_SIZE);
//    connect(this, SIGNAL(BeClick(QPoint)), selectedChess, SLOT(Move(QPoint)));
}


void Path::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug()<<"path mousePressEvent";
    emit BeClicked(pathPos);
}
