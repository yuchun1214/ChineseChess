#include "text.h"

Text::Text(QString s, QGraphicsItem * parent) : QGraphicsTextItem (parent){
        setPlainText(s);
//        setDefaultTextColor(Qt::red);
        setFont(QFont("times",26));
        setX(400);
        setY(0);
}


