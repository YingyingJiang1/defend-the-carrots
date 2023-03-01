#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "figure.h"
#include "player.h"
#include "programplayer.h"

#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QPainter>
#include <QLabel>
#include <QTimerEvent>

extern int extraHeight;

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    ~PlayScene();
    QLabel* addLabel(int x, int y, int width, int height, QString filename, double scale=1.0);
    QPushButton* addButton(int width, int height, int x, int y, QString text);
    void checkAffixDrops();
    pair<int, int> getPosInWindow(int row, int col);
    pair<int, int> getLocInFigure(int x, int y);
    void installAffix(int i, QPushButton* btn);
    void loadFigure();
    void deleteFigure();
    void loadMonsters(vector<Monster*>& monsters);
    void chooseTowerScene(int x, int y);
    void genTowersBtn(int x, int y);
    void killAllTimers();
    void setBtn(QPushButton* btn, int x, int y, QString& filename);
    void deleteChooseTowerScene();
    void genMonsterName(QString& name, Monster* monster);
    void genTowerName(QString& name, Tower* tower);
    void monsterAttack();
    void timerEvent(QTimerEvent *event);
    void showPrompt(int x, int y, QString text);
    void showGameOver(QString image);
    void reset();
    void towerAttack();
    bool prepare(QString& figurePath);
    void deployTowerEvent();

    void genMoneyLabel();
    void genWavesLabel();
    void addAffixSlot(Tower* tower, int x, int y);
    void getBulletTrajectory(int x1, int y1, int x2, int y2, QList<QPair<int, int>>&trajectory);
private:
    const static int barHeight = 80;    // height of picture at the top of window
    int btnWidth, btnHeight;
    QLabel* moneyLabel, *wavesLabel;
    Figure* figure;
    Player* player;
    ProgramPlayer* pPlayer;
    QLabel* pre;
    QLabel* prompt;
    QList<QPushButton*> selectTowerBtns;

    int timerid;  // timer for generating monsters
    int timerid1;     // timer for monsters moving
    int timerid2;   // timer for attack event

    bool gameover;
    bool affixInstallMode;
    bool affixUninstallMode;
    Tower* targetTower; // The target tower for which affix is to be installed.
    int targetAffixSlot;    // The target slot where the affix is to be installed.
    QList<QPushButton*> affixBtns;
    QList<int>affixNum;

    QList<QList<QPair<int, int>>>bullets;  // Bullet trajectory

    static QList<QString> towerImages;
    static QList<QString> monsterImages;
signals:
    void clickBackBn();
};



inline pair<int, int> PlayScene::getPosInWindow(int row, int col)
{
    return make_pair(col*btnWidth, (extraHeight>>1)+row*btnHeight);
}

inline pair<int, int> PlayScene::getLocInFigure(int x, int y)
{
    return make_pair((y-(extraHeight>>1))/btnHeight, x/btnWidth);
}

inline void PlayScene::deleteFigure()
{
    QList<QLabel*> labelList = findChildren<QLabel*>("cover", Qt::FindDirectChildrenOnly);
    labelList.append(findChildren<QLabel*>("addTower", Qt::FindDirectChildrenOnly));
    for(int i = 0; i < labelList.size(); ++i)
        delete labelList[i];
}

inline void PlayScene::setBtn(QPushButton* btn,int x, int y, QString& filename)
{
    btn->setGeometry(x, y, btnWidth>>1, btnHeight>>1);
    btn->setIcon(QIcon(filename));
    btn->setIconSize(QSize(btnWidth>>1, btnHeight>>1));
}

inline void PlayScene::deleteChooseTowerScene()
{
    if(pre)
    {
        delete pre;
        pre = nullptr;
        for(int i = 0; i < selectTowerBtns.size(); ++i)
            delete selectTowerBtns[i];
        selectTowerBtns.clear();
    }
}

inline void PlayScene::genMonsterName(QString &name, Monster *monster)
{
    name = "M" + QString::number(monster->getId());
}

inline void PlayScene::showGameOver(QString image)
{
    QLabel* label = addLabel(0, 0, width(), height(), image, 0.7);
    label->setVisible(true);
}

inline void PlayScene::genMoneyLabel()
{
    moneyLabel = new QLabel(this);
    QPalette label_pe;
    QFont ft;
    ft.setPointSize(20);
    label_pe.setColor(QPalette::WindowText, Qt::darkYellow);
    moneyLabel->setPalette(label_pe);
    moneyLabel->setFont(ft);
    moneyLabel->setAlignment(Qt::AlignCenter);
    moneyLabel->move(width()/20, barHeight*0.2);
    moneyLabel->show();
}

inline void PlayScene::genWavesLabel()
{
    wavesLabel = new QLabel(this);
    wavesLabel->setFixedWidth(150);
    QPalette label_pe;
    QFont ft;
    ft.setPointSize(15);
    label_pe.setColor(QPalette::WindowText, Qt::white);
    wavesLabel->setPalette(label_pe);
    wavesLabel->setFont(ft);
    wavesLabel->setAlignment(Qt::AlignCenter);
    wavesLabel->move((width()>>1) - (wavesLabel->width()>>1), barHeight*0.2);
    wavesLabel->show();
}





#endif // PLAYSCENE_H
