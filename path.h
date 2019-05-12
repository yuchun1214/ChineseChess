#ifndef PATH_H
#define PATH_H

#define PATH_SIZE 30

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>

#include "chess.h"


class Path : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Path(QObject *parent = nullptr);
    explicit Path(QPoint path_pos, Chess * selectedChess, QObject * parent = nullptr);
signals:
    void BeClicked(QPoint);
public slots:

private:
    QPoint pathPos;


    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PATH_H
