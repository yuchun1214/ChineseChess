#ifndef CHESS_H
#define CHESS_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QString>
#include <QPoint>
#include <QVector>
#include <QMouseEvent>


#define BLOCK_SIZE 50
#define CHESS_SIZE 40


class Chess : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Chess(QObject *parent = nullptr);
    explicit Chess(
            QString name,
            QPoint pos,
            QVector<int> x,
            QVector<int> y,
            QVector<QPoint> movingVector,
            QVector<QPoint> addingVector,
            QString resPath,
            char flag,
            bool surpass = false,
            QObject * parent = nullptr
            );
    QString Name();
    QPoint RelativePos();
    QVector<QPoint> MovingVector();
    QVector<int> BorderX();
    QVector<int> BorderY();
    void SET_GLOBAL_AREA_BORDER();
    bool Surpass();
    char Flag();
    void addingAddingVector();
    QVector<QPoint> barriers;

signals:
    void TestSignals();
    void BeClicked(Chess *);
    void HaveMove();

public slots:
    void TestSlots();
    void Move(QPoint selectedPos);

private:
    QString name;
    QPoint relativePos;
    QVector<int> borderX;
    QVector<int> borderY;
    QVector<QPoint> movingVector;
    QVector<QPoint> addingVector;
    char flag;
    bool surpass;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CHESS_H
