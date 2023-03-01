#include "mainscene.h"
#include "mybutton.h"
#include "choosefiguremode.h"
#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QDialog>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(900, 600);
    setWindowIcon(QIcon(":/items/hlb10.png.png"));
    setWindowTitle("Defending radish");

    int bnWidth = this->width()/5, bnHeight = this->height()/10;
    int x = (this->width()>>1)-(bnWidth >>1), y = this->height()*0.8;

    MyButton* stbn = new MyButton(":/buttons/ss_play_normal_CN.png",":/buttons/ss_play_pressed_CN.png",
                                   this, bnWidth, bnHeight, x, y);    
    playScene = new PlayScene;
    chooseFigureMode = new ChooseFigureMode(width() >> 1, height() >> 1);

    // If users finish inputing, then start game.
    connect(chooseFigureMode, &ChooseFigureMode::inputFinished, this, [=](){
        bool success = playScene->prepare(chooseFigureMode->getFigurePath());
        chooseFigureMode->reset();
        if(success)
        {
            chooseFigureMode->hide();
            hide();
            playScene->show();
        }
        else
        {
            chooseFigureMode->importFailure();
        }
    });


    // If click 'start game' button, show play scene.
    connect(stbn, &MyButton::clicked, this, [=](){
        QTimer::singleShot(200, this, [=](){
            chooseFigureMode->show();
        });
    });


    // Listen for the back button signal of the play scene.
    connect(playScene, &PlayScene::clickBackBn, this, [=](){
        playScene->hide();
        this->show();
    });
}

void MainScene::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/bg/mainbg.png"));
}



MainScene::~MainScene()
{
//    delete chooseFigureMode;
//    delete playScene;

}

