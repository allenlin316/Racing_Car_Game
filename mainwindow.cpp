#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#define BLOCK1_X_POS 300
#define BLOCK1_Y_POS 40
#define BLOCK2_X_POS 2300
#define BLOCK2_Y_POS 110
#define BLOCK3_X_POS 900
#define BLOCK3_Y_POS 190
#define BLOCK4_X_POS 2800
#define BLOCK4_Y_POS 260
#define BLOCK5_X_POS 1700
#define BLOCK5_Y_POS 310

/*
        0 => init 遊戲被開啟時
        1 => playing 遊戲遊玩時
        2 => pause 遊戲暫停時
        3 => timeout 時間到，遊戲結束時
        4 => die 碰到障礙物，遊戲結束時
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameStatus = 0;
    //隱藏障礙物
    ui->block1->setGeometry(QRect(-100, 0, 64, 64));
    ui->block2->setGeometry(QRect(-100, 0, 64, 64));
    ui->block3->setGeometry(QRect(-100, 0, 64, 64));
    ui->block4->setGeometry(QRect(-100, 0, 64, 64));
    ui->block5->setGeometry(QRect(-100, 0, 64, 64));

    //then we move background to make car look like moving
    bgPos = 0;
    objectTimer = new QTimer(this); //create a timer
    connect(objectTimer, SIGNAL(timeout()), this, SLOT(updateObject()) ); //Qt objctTimer(sender) -> this (receiver)
    objectTimer->start(10); //QTimer 開始跑，就會連接到this的updateObject() (每隔0.1秒更新一次)

    // timer clock starts to run
    time = 30;
    clockTimer = new QTimer(this); //create a timer
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(updateTime())); //Qt objctTimer(sender) -> this (receiver)
    //clockTimer->start(1000); //QTimer 開始跑，就會連接到this的updateTimer() (每隔1秒更新一次)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateObject(){
    //qDebug() << "timeUp";

    bgPos -= 1; //everytime called we move left

    ui->background->setGeometry(QRect(bgPos, 0, 4800, 512)); //setup background position

    if(bgPos == -32){
        bgPos = 0;                          // 背景中的一棵樹等於32個點，所以過完一棵樹即重新整理畫面
    }

    if(gameStatus == 1){
        //playing
        carDistance += 1;
        ui->lblDistance->setText("行駛距離 : " + QString::number(carDistance) + " 公尺");
        moveCar();
        moveBlocks();
        detectBlocks();
    }
}

void MainWindow::updateTime(){
    time -= 1;

    ui->lcdClock->display(time);

    if(time == 0){
        gameStatus = 3;
        gameStop();
    }
}

void MainWindow::gameStop(){
    clockTimer->stop();
    objectTimer->stop();

    if(gameStatus == 3){
        ui->lblTitle->setPixmap(QPixmap(":/src/game_timeout.png"));
    }
    else if(gameStatus == 4){
        ui->lblTitle->setPixmap(QPixmap(":/src/game_falied.png"));
    }
    ui->lblTitle->setVisible(true);
    ui->buttonStart->setVisible(true);

    ui->action_Start_Game->setText("開始遊戲");
}

void MainWindow::on_buttonStart_clicked() {
    gameStart();
}

void MainWindow::gameStart(){
    gameStatus = 1;             // 1 為 playing status
    ui->lblTitle->setVisible(false);
    ui->buttonStart->setVisible(false);         // make button disappear

    clockTimer->start(1000);
    time = 30;
    ui->lcdClock->display(time);

    carPos = 220;                                           //initialize car position、distance、direction
    carDistance = 0;
    carDirection = 0;

    if(!objectTimer->isActive()){
        //物體計時器，沒開啟則開啟
        objectTimer->start(10);
    }
    ui->action_Start_Game->setText("暫停遊戲");
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {              //這樣就可以偵測到鍵盤的輸入
        case Qt::Key_Up:
            carDirection = -1;
            qDebug() << "up";
            break;
        case Qt::Key_Down:
            carDirection = 1;
            qDebug() << "down";
            break;
    }
}

void MainWindow::moveCar(){
    int carNewPos = carPos + carDirection; //將要移動的位置站存起來

    if( carNewPos >= 30 && carNewPos <= 410 ){      //未超過邊緣
        carPos = carNewPos;
        ui->lblCar->setGeometry(QRect(10, carPos, 237, 71));
    }
    else{
        gameStatus = 4;
        gameStop();
    }
}

void MainWindow::moveBlocks(){
    ui->block1->setGeometry(QRect(BLOCK1_X_POS - carDistance, BLOCK1_Y_POS, 64, 64)); //使障礙物跟著動
    ui->block2->setGeometry(QRect(BLOCK2_X_POS - carDistance, BLOCK2_Y_POS, 64, 64));
    ui->block3->setGeometry(QRect(BLOCK3_X_POS - carDistance, BLOCK3_Y_POS, 64, 64));
    ui->block4->setGeometry(QRect(BLOCK4_X_POS - carDistance, BLOCK4_Y_POS, 64, 64));
    ui->block5->setGeometry(QRect(BLOCK5_X_POS - carDistance, BLOCK5_Y_POS, 64, 64));
}

void MainWindow::detectBlocks(){
    int tmpPaddingX, tmpPaddingY;

    tmpPaddingX = BLOCK1_X_POS - carDistance;
    tmpPaddingY = BLOCK1_Y_POS - ui->lblCar->y();
    if(isCollision(tmpPaddingX, tmpPaddingY)){
        return;
    }
    tmpPaddingX = BLOCK2_X_POS - carDistance;
    tmpPaddingY = BLOCK2_Y_POS - ui->lblCar->y();
    if(isCollision(tmpPaddingX, tmpPaddingY)){
        return;
    }
    tmpPaddingX = BLOCK3_X_POS - carDistance;
    tmpPaddingY = BLOCK3_Y_POS - ui->lblCar->y();
    if(isCollision(tmpPaddingX, tmpPaddingY)){
        return;
    }
    tmpPaddingX = BLOCK4_X_POS - carDistance;
    tmpPaddingY = BLOCK4_Y_POS - ui->lblCar->y();
    if(isCollision(tmpPaddingX, tmpPaddingY)){
        return;
    }
    tmpPaddingX = BLOCK5_X_POS - carDistance;
    tmpPaddingY = BLOCK5_Y_POS - ui->lblCar->y();
    if(isCollision(tmpPaddingX, tmpPaddingY)){
        return;
    }
}

bool MainWindow::isCollision(int x, int y){
    if( (x < 240) && (x > -50) ){
        if( (y < 55)  && (y > -55) ){
            gameStatus = 4;
            gameStop();
            return true;
        }
    }
    return false;
}

void MainWindow::on_action_Start_Game_triggered() {
    switch(gameStatus){
        case 0:     //init
        case 3:     //timeout
        case 4:     //die
            gameStart();
            break;
        case 1:     //playing
        case 2:     //pause
            gamePause();
            break;
    }
}

void MainWindow::gamePause(){
    if(gameStatus == 1){
        gameStatus = 2;

        clockTimer->stop();
        objectTimer->stop();

        ui->lblTitle->setPixmap(QPixmap(":/src/game_pause.png"));
        ui->lblTitle->setVisible(true);
        ui->action_Start_Game->setText("開始遊戲");     //設定選單按鈕
    }
    else if(gameStatus == 2){
        // pause 變為 playing
        gameStatus = 1;

        clockTimer->start(1000);
        objectTimer->start(10);

        ui->lblTitle->setVisible(false);
        ui->action_Start_Game->setText("暫停遊戲");     //設定選單按鈕
    }
}
void MainWindow::on_actionStop_triggered(){
    close();
}
