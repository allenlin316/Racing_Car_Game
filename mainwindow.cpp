#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>

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
