#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BLOCK_SIZE 50
#define PATH_SIZE 30
#define SCENE_POS -100
#define ERRONEOUSX -97
#define ERRONEOUSY -95

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QStringList>
#include <QVector>
#include <QList>
#include <QMap>
#include <QPoint>
#include "chess.h"
#include "path.h"
#include "text.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void buttonClick();
    void Test();
    void chessClicked(Chess * selectedChess);
    void pathClicked(QPoint pos);
    void ResetBoard();

private:

    Ui::MainWindow *ui;
    QGraphicsScene * scene;
    Chess * selectedChess;
    QVector<Chess * > RedChesses;
    QVector<Chess *> BlackChess;
    QVector<Path *> paths;
    Chess * RKing,* BKing;
    char flag;
    bool movable;
    Text * text;


    void LoadChessConfigFile(QString fileName, QVector<Chess *> &,char flag);
    void PaintTheBoard(QString ConfigFileName);
    void Decide(QPoint,char);
    void generatePaths();
    bool checkPathBorder(QPoint, QVector<int>, QVector<int>);
    bool hasEnemyChess(QPoint);
    bool hasChess(QPoint);
    bool checkChessPos(QPoint, QVector<Chess *>);
    bool checkKingToKing();
    void DeletePaths();
    void hideChess(Chess *);
    void showWinnerAndGameOver();
    QMap<QString, QVector<QPoint> > infiniteMovingVector(QPoint pos,QChar mode = 'a');

    bool isBigger(int f,int s);
    bool isSmaller(int f, int s);
    void increment(int &i);
    void decrement(int &i);


    // here has a special syntax
    QVector<QPoint> MeetChess(QPoint startPos, void(QPoint::*SetFunction)(int),int(QPoint::*variantCoordinate)()const,int Bound, bool(MainWindow::*commparison)(int,int),void(MainWindow::*indecrement)(int &));
    QVector<Path *> generateCannonPath(QMap<QString, QVector<QPoint> >points);
    QVector<Path *> generateCarPath(QMap<QString, QVector<QPoint> >points);
    void ShowAllEnemyPath();

};

#endif // MAINWINDOW_H
