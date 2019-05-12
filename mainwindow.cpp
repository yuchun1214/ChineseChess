#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene = new QGraphicsScene();
    this->selectedChess = NULL;
    ui->view->setScene(scene);
    ui->view->setFixedSize(800,600);
    this->scene->setSceneRect(SCENE_POS,SCENE_POS,800,600);
//    connect(ui->test,SIGNAL(clicked(bool)),this,SLOT(buttonClick()));
    connect(ui->reset, SIGNAL(clicked(bool)), this, SLOT(ResetBoard()));


    this->PaintTheBoard(QString(":/BoardConfig/lineConfig.json"));
    this->LoadChessConfigFile(QString(":/ChessConfig/RedConfig.json"), RedChesses,'r');
    this->LoadChessConfigFile(QString(":/ChessConfig/BlackConfig.json"),BlackChess,'b');


    for(int i = 0, size = RedChesses.size(); i < size; i++){
        connect(RedChesses[i],SIGNAL(BeClicked(Chess*)),this,SLOT(chessClicked(Chess *)));
    }
    for(int i = 0, size = BlackChess.size(); i < size; i++){
        connect(BlackChess[i], SIGNAL(BeClicked(Chess*)),this, SLOT(chessClicked(Chess *)));
    }

    RKing = RedChesses[0];
    BKing = BlackChess[0];


}

void MainWindow::LoadChessConfigFile(QString fileName, QVector<Chess *> & chesses,char flag)
{
    QFile file;
    QString val;
    QString name;
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray config = d.array();

    QVector<int> borderX;
    QVector<int> borderY;
    QPoint startPos;
    QJsonArray StartPosArray;
    QJsonObject border;
    QJsonArray borderXArray;
    QJsonArray borderYArray;
    QJsonArray movingVectorJSONArray;
    QJsonArray movingVectorElement;
    QJsonArray barriersJsonArray;
    QJsonArray barriersJson;
    QJsonArray startPosElements;
    QVector<QPoint> movingVector;
    QVector<QPoint> barriers;
    QJsonArray addingVectorJSON;
    QJsonArray addingVectorElementJSON;
    QVector<QPoint> addingVector;
    QJsonObject temp;
    QString path;
    bool surpass;
    Chess * newChess;
    for(int i = 0; i < config.size(); i++){
        temp = config[i].toObject();
        name = temp["name"].toString();
        border = temp["border"].toObject();
        borderXArray = border["x"].toArray();
        borderYArray = border["y"].toArray();
        StartPosArray = temp["pos"].toArray();
        surpass = temp["surpass"].toBool();
        path = temp["path"].toString();
        for(int i = 0 , size = borderXArray.size(); i < size; i++){
            borderX.append(borderXArray[i].toInt() * BLOCK_SIZE);
        }
        for(int i = 0, size = borderYArray.size(); i < size; i++){
            borderY.append(borderYArray[i].toInt() * BLOCK_SIZE);
        }

       movingVectorJSONArray = temp["movingVector"].toArray();
       for(int i = 0 ,size = movingVectorJSONArray.size();i < size; i++){
           movingVectorElement = movingVectorJSONArray[i].toArray();
           movingVector.append(QPoint(movingVectorElement[0].toInt() * BLOCK_SIZE, movingVectorElement[1].toInt() * BLOCK_SIZE));
       }

       barriersJson = temp["barrier"].toArray();
       for(int i = 0, size = barriersJson.size(); i < size; i++){
           barriersJsonArray = barriersJson[i].toArray();
           barriers.push_back(QPoint(barriersJsonArray[0].toInt() * BLOCK_SIZE, barriersJsonArray[1].toInt() * BLOCK_SIZE));
       }

       addingVectorJSON = temp["addingVector"].toArray();
       for(int i = 0 ,size = addingVectorJSON.size(); i < size; i++){
           addingVectorElementJSON = addingVectorJSON[i].toArray();
           addingVector.push_back(QPoint(addingVectorElementJSON[0].toInt() * BLOCK_SIZE, addingVectorElementJSON[1].toInt() * BLOCK_SIZE));
       }

       for(int i = 0, size = StartPosArray.size(); i < size; i ++){
            startPosElements = StartPosArray[i].toArray();
            startPos.setX(startPosElements[0].toInt() * BLOCK_SIZE);
            startPos.setY(startPosElements[1].toInt() * BLOCK_SIZE);
            newChess = new Chess(name,startPos,borderX,borderY,movingVector,addingVector,path,flag,surpass);
            newChess->barriers = barriers;
            chesses.append(newChess);
       }

       qDebug()<<"Name : "<<name;
       qDebug()<<"poss : "<<StartPosArray;
       qDebug()<<"borderX : "<<borderX;
       qDebug()<<"borderY : "<<borderY;
       qDebug()<<"surpass : "<<surpass;
       qDebug()<<"movingVector"<<movingVector;
       qDebug()<<"path : "<<path;
       qDebug()<<"barriers : "<<barriers;
       qDebug()<<"adding Vector : "<<addingVector;
       qDebug()<<"";

       movingVector.clear();
       borderX.clear();
       borderY.clear();
       barriers.clear();
       addingVector.clear();
    }

    for(int i = 0, size = chesses.size(); i < size; i++){
        this->scene->addItem(chesses[i]);
    }
}

void MainWindow::PaintTheBoard(QString ConfigFileName)
{
    QFile file;
    QString val;
    file.setFileName(ConfigFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject config = d.object();
    QGraphicsLineItem * lineItem;

    // construct the horizontal line
    QJsonArray type;
    QJsonArray aline;
    QJsonArray pointArray1;
    QJsonArray pointArray2;


    QStringList keys = config.keys();
    for(int i = 0, keySize = keys.size(); i < keySize; i++){
        type =  config[keys[i]].toArray();
        for(int j = 0, size = type.size(); j < size; j++){
            aline = type[j].toArray();
    //        qDebug()<<aline;
            pointArray1 = aline[0].toArray();
            pointArray2 = aline[1].toArray();
    //        qDebug()<<pointArray1;
            lineItem = new QGraphicsLineItem();
            lineItem->setLine(pointArray1[0].toInt() * BLOCK_SIZE, pointArray1[1].toInt() * BLOCK_SIZE, pointArray2[0].toInt() * BLOCK_SIZE, pointArray2[1].toInt() * BLOCK_SIZE);
            scene->addItem(lineItem);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event){

}

void MainWindow::buttonClick(){

    qDebug()<<"buttonClick";
    qDebug()<<checkKingToKing();
;
}

void MainWindow::Test()
{
    qDebug()<<"Test is called";
}

void MainWindow::generatePaths(){
//    qDebug()<<"in generatePaths";

    Path * path;
    if(selectedChess){
        QVector<QPoint> mv = this->selectedChess->MovingVector();
        QVector<QPoint> barriers = this->selectedChess->barriers;
        QPoint chessPos = this->selectedChess->RelativePos();
        QVector<int> borderX = this->selectedChess->BorderX();
        QVector<int> borderY = this->selectedChess->BorderY();
//        qDebug()<<borderX;
//        qDebug()<<borderY;
        QPoint pathPos;
        QPoint barriersPos;
        bool checkBorder;
        if(mv.size() && !barriers.size()){
            for(int i = 0, size = mv.size(); i < size; i++){
                pathPos = chessPos + mv[i];
                qDebug()<<pathPos;
                checkBorder = checkPathBorder(pathPos, borderX, borderY);
                if(!checkBorder && this->selectedChess->Surpass()){
                    qDebug()<<"in if 1";
                    path = new Path(pathPos, this->selectedChess);
                    this->paths.append(path);
                    this->selectedChess->addingAddingVector();

                }else if(checkBorder){
                    qDebug()<<"in if 2";
                    path = new Path(pathPos , this->selectedChess);
                    this->paths.append(path);

                }
            }
        }
        else if(mv.size() && barriers.size()){
            for(int i = 0, size = mv.size(); i < size; i++){
                pathPos = chessPos + mv[i];
                barriersPos = chessPos + barriers[i];
                qDebug()<<barriersPos;
                if(!hasChess(barriersPos) && checkPathBorder(pathPos, borderX, borderY)){
                    path = new Path(pathPos, this->selectedChess);
                    this->paths.append(path);
                }

            }
        }
        else {
            if(this->selectedChess->Name() == QString("Car")){
                qDebug()<<this->selectedChess->Flag();
                qDebug()<<this->selectedChess->Name();
                QMap<QString, QVector<QPoint> > infiniteMv = this->infiniteMovingVector(selectedChess->RelativePos());
                generateCarPath(infiniteMv);
            }else if(this->selectedChess->Name() == QString("Cannon")){
                qDebug()<<this->selectedChess->Name();
                QMap<QString, QVector<QPoint> > infiniteMv = this->infiniteMovingVector(selectedChess->RelativePos());
                generateCannonPath(infiniteMv);
            }
        }
    }

    for(int i = 0, size = this->paths.size(); i < size; i++){
        connect(this->paths[i],SIGNAL(BeClicked(QPoint)),this,SLOT(pathClicked(QPoint)));
        scene->addItem(this->paths[i]);
    }
//    paths.clear();
//    qDebug()<<"";
}

bool MainWindow::checkPathBorder(QPoint pathPos, QVector<int> borderX, QVector<int> borderY){
//    qDebug()<<"in checkPathBorder";
    if(pathPos.x() >= borderX[0] && pathPos.x() <= borderX[1] && pathPos.y() >= borderY[0] && pathPos.y() <= borderY[1]){
        return hasEnemyChess(pathPos);
//        return true;
    }
    return false;
}

bool MainWindow::hasEnemyChess(QPoint relativePos){
    int objectAmount = ui->view->items(QPoint(relativePos.x() - ERRONEOUSX, relativePos.y() - ERRONEOUSY)).size();
//    qDebug()<<"in hasChess "<<objectAmount;
//    return objectAmount == 1 ? false : true;
    if(!objectAmount)
        return true;
    else {
        // find is red's chess or black's chess
        char flag = selectedChess->Flag();
        if(flag == 'r')
            return checkChessPos(relativePos, BlackChess);
        else if(flag == 'b')
            return checkChessPos(relativePos, RedChesses);
    }
    return false;
}

bool MainWindow::hasChess(QPoint pos)
{
    if(this->ui->view->items(QPoint(pos.x() - ERRONEOUSX, pos.y() - ERRONEOUSY)).size())
        return true;
    return  false;
}

bool MainWindow::checkChessPos(QPoint pos, QVector<Chess *> chesses){
    for(int i = 0, size = chesses.size(); i < size; i++){
        if(pos == chesses[i]->RelativePos())
            return true;
    }
    return false;
}

bool MainWindow::checkKingToKing(){
    QMap<QString, QVector<QPoint> > meets;
    qDebug()<<"\ncheckKingToKing\n";
    meets = infiniteMovingVector(RKing->RelativePos(), 'd');
    qDebug()<<meets;
    if(meets[QString("down")].last() == BKing->RelativePos())
        return  false;
    return true;
//    qDebug()<<meets;
}

void MainWindow::hideChess(Chess * c){
    QGraphicsItem * temp;
    if(c){
        temp = this->ui->view->itemAt(QPoint(c->RelativePos().x() - ERRONEOUSX, c->RelativePos().y() - ERRONEOUSY));
        if(temp)
            temp->hide();
    }
}

void MainWindow::chessClicked(Chess * selectedChess){
    qDebug()<<"called chess Clicked";
    this->selectedChess = selectedChess;
    qDebug()<<selectedChess->Name();
    this->DeletePaths();
    this->Decide(selectedChess->RelativePos(),'c');
}

void MainWindow::DeletePaths(){
    for(int i = 0, size = paths.size(); i < size; i++){
        delete paths[i];
    }
    paths.clear();
}

void MainWindow::pathClicked(QPoint pos){
//    qDebug()<<"pathClicked\n";
    this->Decide(pos,'p');
}

QMap<QString, QVector<QPoint> > MainWindow::infiniteMovingVector(QPoint pos,QChar mode){
    int objectAmounts;
    QMap<QString, QVector<QPoint> > meets;
   if(mode == 'r')
        meets["right"] = this->MeetChess(pos,&QPoint::setX, &QPoint::x, 400, &MainWindow::isSmaller,&MainWindow::increment);
   else if(mode == 'l')
       meets["left"] = this->MeetChess(pos,&QPoint::setX, &QPoint::x, 0, &MainWindow::isBigger, &MainWindow::decrement);
   else if(mode == 'u')
       meets["up"] = this->MeetChess(pos,&QPoint::setY, &QPoint::y,0,&MainWindow::isBigger, &MainWindow::decrement);
   else if(mode == 'd')
       meets["down"] = this->MeetChess(pos,&QPoint::setY, &QPoint::y,450,&MainWindow::isSmaller, &MainWindow::increment);
   else {
       meets["right"] = this->MeetChess(pos,&QPoint::setX, &QPoint::x, 400, &MainWindow::isSmaller,&MainWindow::increment);
       meets["left"] = this->MeetChess(pos,&QPoint::setX, &QPoint::x, 0, &MainWindow::isBigger, &MainWindow::decrement);
       meets["up"] = this->MeetChess(pos,&QPoint::setY, &QPoint::y,0,&MainWindow::isBigger, &MainWindow::decrement);
       meets["down"] = this->MeetChess(pos,&QPoint::setY, &QPoint::y,450,&MainWindow::isSmaller, &MainWindow::increment);
   }
   return meets;
}

bool MainWindow::isBigger(int f, int s){
    return  f >= s;
}

bool MainWindow::isSmaller(int f, int s){
    return  f <= s;
}

void MainWindow::increment(int &i){
    i += BLOCK_SIZE;
}

void MainWindow::decrement(int &i){
    i -= BLOCK_SIZE;
}

QVector<QPoint> MainWindow::MeetChess(QPoint startPos, void(QPoint::*SetFunction)(int),int(QPoint::*variantCoordinate)()const, int Bound, bool(MainWindow::*commparison)(int,int),void(MainWindow::*indecrement)(int &)){
    QPoint pos = startPos;
    int objectAmounts;
    int i = (pos.*variantCoordinate)();
    QVector<QPoint> posses;
    for((this->*indecrement)(i); (this->*commparison)(i,Bound); (this->*indecrement)(i)){
        (pos.*SetFunction)(i);
        posses.append(pos);
        objectAmounts = ui->view->items(QPoint(pos.x() - ERRONEOUSX, pos.y() - ERRONEOUSY)).size();
        if(objectAmounts)
            return posses;
    }
    return posses;
}

QVector<Path *> MainWindow::generateCannonPath(QMap<QString, QVector<QPoint> > points)
{
    QList<QString> keys = points.keys();
    QMap<QString, QVector<QPoint> > infiniteMv;
    QPoint lastPoint;
    QPoint attackPoint;
    Path * path;
    QPoint tempp;
    int objectAmounts;
    for(int i = 0 ; i < keys.size(); i++){
        if(!points[keys[i]].isEmpty()){
            lastPoint = points[keys[i]].last();
            objectAmounts = ui->view->items(QPoint(lastPoint.x() - ERRONEOUSX, lastPoint.y() - ERRONEOUSY)).size();
            if(objectAmounts)
                points[keys[i]].pop_back();
            infiniteMv = infiniteMovingVector(lastPoint,keys[i][0]);
        }
        if(!infiniteMv[keys[i]].isEmpty()){
            tempp = infiniteMv[keys[i]].last();
            objectAmounts = ui->view->items(tempp.x() - ERRONEOUSX, tempp.y() - ERRONEOUSY).size();
            if(hasEnemyChess(tempp) && objectAmounts){
                qDebug()<<tempp;
                points[keys[i]].push_back(tempp);
            }
        }
    }

    qDebug()<<"finish inifnteMv";

    for(int i = 0; i < keys.size(); i++){
        for(int j = 0 ; j < points[keys[i]].size(); j ++){
            qDebug()<<points[keys[i]][j];
            path = new Path(points[keys[i]][j],selectedChess);
            this->paths.append(path);
        }
    }
    return this->paths;
}

QVector<Path *> MainWindow::generateCarPath(QMap<QString, QVector<QPoint> > points)
{
    qDebug()<<"generateCarPath";
    QList<QString> keys = points.keys();
//    QVector<Path *> paths;
    Path * path;
    QPoint lastPoint;
    for(int i = 0; i < keys.size(); i++){
        if(!points[keys[i]].isEmpty()){
            lastPoint = points[keys[i]].last();
            if(!hasEnemyChess(lastPoint)){
                points[keys[i]].pop_back();
            }
        }
    }
    for(int i = 0; i < keys.size(); i++){
        for(int j = 0 ; j < points[keys[i]].size(); j ++){
            path = new Path(points[keys[i]][j],selectedChess);
            this->paths.append(path);
        }
    }
    return this->paths;
}

void MainWindow::Decide(QPoint selectedPos,char who){
    int objectAmount = ui->view->items(QPoint(selectedPos.x() - ERRONEOUSX, selectedPos.y() - ERRONEOUSY)).size();
    qDebug()<<"objectAmount" << objectAmount;
    qDebug()<<"who is "<<who<<"\n";
    if(objectAmount == 1){
        if(who == 'c'){
            // create the path
            generatePaths();

        }else if(who == 'p'){
            // move the chess
            qDebug()<<"chess move";
            QPoint temp = this->selectedChess->RelativePos();
            this->selectedChess->Move(selectedPos);
            this->DeletePaths();
            if(!checkKingToKing()){

                this->selectedChess->Move(temp);
            }
        }
    }else if(objectAmount == 2){
        // means that there are two objects on the same pos, one is path and another is enemy
//        qDebug()<<"objectAmount is 2";
//        qDebug()<<"==========="<<selectedPos<<"============";
        char flag = selectedChess->Flag();
        QVector<Chess *> & chooseEnemy = (flag == 'r' ? BlackChess : RedChesses);
        for(QVector<Chess *>::iterator it = chooseEnemy.begin(); it != chooseEnemy.end(); it++){
            if((*it)->RelativePos() == selectedPos){
                delete  *it;
                chooseEnemy.erase(it);
                break;
            }
        }
        this->selectedChess->Move(selectedPos);
        this->DeletePaths();
    }

}

void MainWindow::ShowAllEnemyPath(){
    Chess * temp = selectedChess;
    for(int i = 0 ; i < RedChesses.size(); i++){
        selectedChess = RedChesses[i];
        generatePaths();
    }
    selectedChess = temp;
}

void MainWindow::ResetBoard(){
    // reset the chess;
    qDebug()<<"in reset board";
    qDebug()<<RedChesses.size();
    qDebug()<<BlackChess.size();
    for(int i = 0, size = RedChesses.size(); i < size; i++){
        delete RedChesses[i];
    }
    RedChesses.clear();

    for(int i = 0, size = BlackChess.size(); i < size; i++){
        delete BlackChess[i];
    }
    BlackChess.clear();
    qDebug()<<"Reset the chess safe";

    // reset the path
    for(int i = 0, size = paths.size(); i < size; i++){
        delete paths[i];
    }
    paths.clear();
    qDebug()<<"Reset the paths safe";

    // reset the selectChess, Kings
    selectedChess = RKing = BKing = nullptr;
    qDebug()<<"reset selectedChess safe";

    // reload the chess
    this->LoadChessConfigFile(QString(":/ChessConfig/RedConfig.json"), RedChesses,'r');
    this->LoadChessConfigFile(QString(":/ChessConfig/BlackConfig.json"),BlackChess,'b');


    for(int i = 0, size = RedChesses.size(); i < size; i++){
        connect(RedChesses[i],SIGNAL(BeClicked(Chess*)),this,SLOT(chessClicked(Chess *)));
    }
    for(int i = 0, size = BlackChess.size(); i < size; i++){
        connect(BlackChess[i], SIGNAL(BeClicked(Chess*)),this, SLOT(chessClicked(Chess *)));
    }

    RKing = RedChesses[0];
    BKing = BlackChess[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}
