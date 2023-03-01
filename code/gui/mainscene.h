#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "playscene.h"
#include "choosefiguremode.h"
#include <QMainWindow>
#include <QCloseEvent>

class MainScene : public QMainWindow
{
    Q_OBJECT
public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    void paintEvent(QPaintEvent* event);
private:
    PlayScene* playScene;
    ChooseFigureMode* chooseFigureMode;

};
#endif // MAINSCENE_H
