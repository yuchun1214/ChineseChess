#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsTextItem>
#include <QFont>

class Text : public QGraphicsTextItem{
public:
    Text(QString s,QGraphicsItem * parent = 0);
};


#endif // TEXT_H
