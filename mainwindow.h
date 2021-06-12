#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *); //keyboard 的監聽

private slots:
    void updateTime();     // update GameTime
    void updateObject(); //遊戲物體移動更新

    void on_buttonStart_clicked();

    void on_action_Start_Game_triggered();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;

    int gameStatus;
    int bgPos;                      //background position (horizontal)
    int time;
    int carPos;                     //record car position
    int carDirection;           // car's direction
    float carDistance;

    QTimer *objectTimer;    //object moving timer
    QTimer *clockTimer;

    void gameStart();
    void gamePause();
    void gameStop();
    void moveCar();
    void moveBlocks();  //障礙物
    void detectBlocks();
    bool isCollision(int x, int y);     //判斷是否撞到
};
#endif // MAINWINDOW_H
