#include "playscene.h"
#include "mybutton.h"
#include <QPixmap>
#include <QTimer>
#include <QPalette>
#include <QDebug>
#include <QHBoxLayout>
#include <iostream>
using namespace std;


int extraHeight = 200;

QList<QString> PlayScene::towerImages = {":/towers/Bottle11.png", ":/towers/Shit11.png", ""};
QList<QString> PlayScene::monsterImages = {":/monsters/land_star01.png", ":/monsters/land_pink01.png", ":/monsters/fly_blue01.png"};


PlayScene::PlayScene(QWidget *parent)
    : QMainWindow{parent}
{   
    setFixedSize(900, 600+extraHeight);
    setWindowTitle("In-game");
    setWindowIcon(QIcon(":/items/hlb10.png.png"));

    // Assign value to data members of class.
    int rows,  cols;
    pre = nullptr;
    figure = new Figure;
    player = new Player;
    rows = figure->getRows();
    cols = figure->getCols();
    btnWidth = width()/cols, btnHeight = (height()-extraHeight)/rows;
    pPlayer = new ProgramPlayer(figure, btnWidth, btnHeight);
    gameover = false;
    genMoneyLabel();
    genWavesLabel();
    prompt = new QLabel(this);
    affixInstallMode = false;
    affixUninstallMode = false;
    affixNum.resize(AFFIX_KINDS, 0);


    // Add back button on picture which is at the top of window.
    int bnWidth = width()>>5, bnHeight = barHeight*0.8;
    int x = width() - (bnWidth << 1), y = 0;
    MyButton* backBn = new MyButton(":/buttons/ss_back_normal.png",":/buttons/ss_back_pressed.png",
                                    this,bnWidth, bnHeight, x, y);

    // Add a button to export figure.
    bnWidth <<= 1;
    x = x - bnWidth - 10;
    MyButton* exportFigure = new MyButton(":/buttons/theend1_btn_normal.png",":/buttons/theend1_btn_press.png",

                                          this,bnWidth, bnHeight, x, 5);

    // Add buttons to install or uninstall affix.
    affixBtns.push_back(new QPushButton("狂暴:0", this));
    affixBtns.push_back(new QPushButton("冰系:0", this));
    affixBtns.push_back(new QPushButton("群伤:0", this));
    affixBtns.push_back((new QPushButton("放血:0", this)));  
    for(int i = 0; i < AFFIX_KINDS; ++i)
    {
        QPushButton* tmp = affixBtns[i];
        tmp->setFixedSize(bnWidth, bnHeight);
        x = x - bnWidth - 5;
        tmp->move(x, 5);
        connect(tmp, &QPushButton::clicked, this, [=](){
            installAffix(i, tmp);
        });
    }


    // Add text on button.
    QLabel* label = new QLabel(this);
    label->setText("导出地图");
    label->setFixedSize(exportFigure->size());
    label->setAlignment(Qt::AlignCenter);
    label->move(exportFigure->pos());
    label->setAttribute(Qt::WA_TransparentForMouseEvents);


    // Connect button: back button, export figure button.
    connect(exportFigure, &MyButton::clicked, this, [=](){
        affixInstallMode = false;
        bool ret = figure->exportFigure();
        int x = exportFigure->pos().x(), y = exportFigure->pos().y() + 80;
        if(ret)
            showPrompt(x, y, "导出成功！");
        else
            showPrompt(x, y, "导出失败！");
    });
    connect(backBn, &MyButton::clicked, this, [=](){
        reset();
        QTimer::singleShot(200, this, [=](){});
        emit clickBackBn();

        // Following implementation will cause the program to crash. It's so strange!
//        QTimer::singleShot(200, this, [=](){
//            emit clickBackBn();
//        });
    });    

    // Deploy rows*cols buttons in window.          
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            QPushButton* btn;
            int x = j*btnWidth, y = (extraHeight>>1) + i*btnHeight;
            btn = new QPushButton(this);
            btn->setFixedSize(btnWidth, btnHeight);
            btn->move(x, y);
            connect(btn, &QPushButton::clicked, this, [=](){
                affixInstallMode = false;
                if(!gameover)
                    chooseTowerScene(btn->pos().x(), btn->pos().y());
            });
        }
    }


}


PlayScene::~PlayScene()
{
    delete figure;
    delete player;
    delete pPlayer;
}


/*
 * @param:
 *        x, y: position of the button to be covered in window
 *        width, height: width and height of the picture
 *        filename: filename of the png to be added on the button
 *        scale: the scale factor of the picture
 *
 *@notes: If you do not reset the name of the label, the label will be deleted when you click the Back button.
*/
QLabel* PlayScene::addLabel(int x, int y,int width, int height, QString filename, double scale)
{
    QLabel* label = new QLabel(this);
    QPixmap pix = QPixmap(filename);
    pix = pix.scaled(width*scale, height*scale,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setObjectName("cover");
    label->setFixedSize(width, height);
    label->setPixmap(pix);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    label->setAlignment(Qt::AlignCenter);
    label->move(x, y);
    return label;
}


void PlayScene::installAffix(int i, QPushButton *btn)
{
    int num = affixNum[i];
    if(affixInstallMode)
    {
        affixInstallMode = false;
        int flag;
        if(num > 0)
        {
            flag = targetTower->installAffix(i+1, targetAffixSlot);
            if(flag == 3)
            {
                --affixNum[i];
                affixBtns[i]->setText(affixName(i+1)+QString::number(affixNum[i]));
            }
            else if(flag == 2)
                showPrompt(btn->pos().x(), btn->pos().y()+50, "已安装过该类型词缀,\n 安装失败！");
            else if(flag == 1)
                showPrompt(btn->pos().x(), btn->pos().y()+50, "远程塔无法安装此类型词缀,\n 安装失败！");
            else if(flag == 0)
                showPrompt(btn->pos().x(), btn->pos().y()+50, "近战塔无法安装此类型词缀,\n 安装失败！");
        }
        else
        {
            showPrompt(btn->pos().x(), btn->pos().y()+50, "词缀数量为0,\n 安装失败！");
        }
    }
}


void PlayScene::addAffixSlot(Tower *tower, int x, int y)
{
    int width = btnWidth, height = btnHeight>>2;
    for(int i = 0; i < AFFIX_SLOTS; ++i)
    {
        QString initBtnName = "词缀"+QString::number(i+1)+":空";
        QPushButton* btn = new QPushButton(initBtnName, this);
        btn->setGeometry(x, y, width, height);
        y = y-height;
        btn->setEnabled(true);
        btn->show();
        tower->setAffixBtn(btn, i);
        connect(btn, &QPushButton::clicked, this, [=](){
            if(tower->emptyAffix(i))
            {
                affixInstallMode = true;
                targetTower = tower;
                targetAffixSlot = i;
            }
            else
            {
                int uninstalledAffix = tower->uninstallAffix(i);
                if(uninstalledAffix > 0)
                {
                    ++affixNum[uninstalledAffix-1];
                    affixBtns[uninstalledAffix-1]->setText(affixName(uninstalledAffix) + QString::number(affixNum[uninstalledAffix-1]));
                }
            }
        });
    }
}


void PlayScene::chooseTowerScene(int x, int y)
{
    deleteChooseTowerScene();

    // If clicks start or end positions or not empty location, then doing nothing.
    pair<int, int>location = getLocInFigure(x, y);
    if(location == figure->getEnd() || figure->notEmptyLocation(location))
        return;
    for(int i = 0; i < figure->getPathsNum(); ++i)
        if(location == figure->getStart(i))
            return;

    // Add picture 'add.png' on the clicked button.
    QLabel* label = new QLabel(this);
    QPixmap pix;
    pix.load(":/buttons/add.png");
    pix = pix.scaled(btnWidth, btnHeight);
    label->setObjectName("addTower");
    label->setFixedSize(btnWidth, btnHeight);
    label->setPixmap(pix);
    label->move(x, y);
    label->show();
    pre = label;

    // Generate and show select tower buttons.
    QList<QString> icons = {":/towers/Bottle01.png", ":/towers/Shit01.png", ":/towers/Fan01.png"};
    QPushButton* btn;
    int dis = (btnWidth>>1) + 10;
    int x1 = x, y1 = y - 50;
    int diff = (width() - x) - ((btnWidth>>1) * numOfTowerKinds + 10 * (numOfTowerKinds-1));
    if(diff < 0)
        x1 += diff;
    for(int i = 0; i < numOfTowerKinds; ++i)
    {
        btn = new QPushButton(this);
        setBtn(btn, x1, y1, icons[i]);
        btn->setObjectName(QString::number(i));

        pair<int, int> location = getLocInFigure(x, y);
        bool onPath = figure->onPath(location);

        if((player->getMoney() < BOTTLE_COST || onPath) && i == 0)
            btn->setEnabled(false);
        else if((player->getMoney() < SHIT_COST || !onPath) && i == 1)
            btn->setEnabled(false);
        if(i == 2)  // Fan tower hasn't been implemented.
            btn->setEnabled(false);        


        // Player click button to deploy towers.
        connect(btn, &QPushButton::clicked, this, [=](){
            int kind = btn->objectName().toInt();
            deleteChooseTowerScene();
            Tower* tower = player->deploy(figure, location, (TowerKind)kind);
            if(tower)
            {
                addAffixSlot(tower, x+10, y - 5);
                repaint();
            }
        });

        btn->show();
        selectTowerBtns.push_back(btn);
        x1 = x1 + dis;
    }

}


inline int getMonsterInitialBlood(MonsterKind kind)
{
    if(kind == STAR_MONSTER)
        return STAR_MONSTER_BLOOD;
    if(kind == PINK_MONSTER)
        return PINK_MONSTER_BLOOD;
    if(kind == FLY_BLUE_MONSTER)
        return FLY_BLUE_MONSTER_BLOOD;
}


inline int getTowerInitialBlood(TowerKind kind)
{
    if(kind == BOTTLE)
        return BOTTLE_BLOOD;
    if(kind == SHIT)
        return SHIT_BLOOD;
    if(kind == FAN)
        return FAN_BLOOD;
}


void PlayScene::paintEvent(QPaintEvent* event)
{   
    // Load background picture
    QPainter painter(this);
    QPixmap image;

    image.load(":/bg/bg1.png");
    painter.drawPixmap(0, 0, width(), height(),  image);

    // Add picture at the top of window
    //QPainter painter1(this);
    image.load(":/bar/MenuBG.png");
    painter.drawPixmap(0, 0, width(), barHeight, image);

    // Add picture at the bottom of window.
    image.load(":/items/ss_cloud.png");
    painter.drawPixmap(0, height()-100, width(), 100, image);

    //Add image on buttons which are on path.
    image.load(":/items/pathItem4.png");
    int pathsNum = figure->getPathsNum();
    for(int i = 0; i < pathsNum; ++i)
    {
        vector<pair<int, int>>& path = figure->getPath(i);
        for(int j = 0; j < path.size(); ++j)
        {
            pair<int, int>pos = getPosInWindow(path[j].first, path[j].second);
            painter.drawPixmap(pos.first, pos.second, btnWidth, btnHeight, image);
        }

    }

    // Paints monsters
    const vector<Monster*>& monsters = const_cast<const vector<Monster*>&>(pPlayer->getMonsters());
    int size = monsters.size();    
    for(int i = 0; i < size; ++i)
    {
        // Paints image of monsters.
        int index = monsters[i]->getKind();
        int width = btnWidth>>1, height =  btnHeight>>1;
        int x = monsters[i]->getX(), y = monsters[i]->getY();
        image.load(monsterImages[index]);
        painter.drawPixmap(x, y, width, height, image);

        // Paints lifeBar for monsters.
        int initialBlood = getMonsterInitialBlood(monsters[i]->getKind());
        float rate = 1.0 * monsters[i]->getBlood() / initialBlood;
        int rectY = y - height*0.2, lifeBarH = height*0.2;
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(x, rectY, width, lifeBarH);
        painter.setBrush(Qt::red);
        painter.drawRect(x, rectY, rate*width, lifeBarH);

        // Paints states for monsters(whether in frozen or in bleeding).
        if(monsters[i]->inFrozen())
        {
            painter.setPen(QColor(Qt::blue));
            painter.drawText(x, y+(height>>2), "冰冻中");
        }
        if(monsters[i]->inBleeding())
        {
            painter.setPen(QColor(Qt::red));
            painter.drawText(x, y+(height>>1), "流血中");
        }

        // Paints affixes for monsters
        painter.setPen(QColor(Qt::black));
        painter.drawText(x, rectY-5, QString::fromStdString(monsters[i]->getAffixName(0)));
        painter.drawText(x, rectY-10, QString::fromStdString(monsters[i]->getAffixName(1)));
    }

    // Paints towers.
    painter.setBrush(Qt::green);
    const vector<Tower*>& towers = player->getTowers();
    int towersNum = towers.size();
    for(int i = 0; i < towersNum; ++i)
    {

        int row = towers[i]->getRow(), col = towers[i]->getCol();
        pair<int, int>pos = getPosInWindow(row, col);
        int index = towers[i]->getKind();
        int width = btnWidth >> 1, height = btnHeight >> 1;
        int x = pos.first + (width>>1), y = pos.second + (height >> 1);

        // Paints images of towers.
        image.load(towerImages[index]);
        painter.drawPixmap(x, y, width, height, image);

        // Paints lifeBar for towers.
        TowerKind kind = towers[i]->getKind();
        int rectY = y-height*0.2, rectH = height*0.2;
        int initialBlood = getTowerInitialBlood(kind);
        float rate = 1.0 * towers[i]->getBlood() / initialBlood;
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(x, rectY, width, rectH);
        painter.setBrush(Qt::green);
        painter.drawRect(x, rectY, rate*width, rectH);        
    }

    // Paints trajectory of bullets.
    image.load(":/items/mcm01.png");
    for(int i = 0; i < bullets.size(); ++i)
    {
        QList<QPair<int, int>>&trajectory = bullets[i];
        for(int j = 0; j < trajectory.size(); ++j)
        {
            painter.drawPixmap(trajectory[j].first, trajectory[j].second,4, 4, image);
        }
    }
    bullets.clear();
    moneyLabel->setText(QString::number(player->getMoney()));

    return QMainWindow::paintEvent(event);
}


void PlayScene::getBulletTrajectory(int x1, int y1, int x2, int y2, QList<QPair<int, int>>& trajectory)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 2 :-2, sy = y1 < y2 ? 2 : -2;
    int err = dx > dy ? dx/2 : -dy/2;
    int e2;
    bool flag1 = x1 < x2 ? true:false, flag2 = !flag1;
    while(true)
    {
        trajectory.append(QPair<int, int>(x1, y1));
        if((flag1 && x1 >= x2) || (flag2 && x1 <= x2))
            break;
        e2 = err;
        if(e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }

}


void PlayScene::monsterAttack()
{
    vector<Tower*>& towers = player->getTowers();
    const vector<Monster*>& monsters = pPlayer->getMonsters();
    int monstersNum = monsters.size();
    for(int i = 0; i < monstersNum; ++i)
    {
        monsters[i]->deductCoolingTime();
        auto it = towers.begin();
        while(it != towers.end())
        {
            if(monsters[i]->inAttackRange(*it))
            {
                monsters[i]->attack(*it);
                if(!(*it)->isAlive())
                {
                    figure->setLocation((*it)->getRow(), (*it)->getCol(), false);
                    delete *it;
                    it = towers.erase(it);
                }
                break; // Each monster can only attacks one tower.
            }
            else
                ++it;
            if(it == towers.end())
                break;
        }

    }

}


void PlayScene::checkAffixDrops()
{
    int rand = myRand(20);
    if(rand >= 1 && rand <= 4)
    {
        affixNum[rand-1] += 1;
        QPushButton* tmp = affixBtns[rand-1];
        tmp->setText(affixName(rand) + QString::number(affixNum[rand-1]));
    }
}


void PlayScene::towerAttack()
{
    const vector<Tower*>& towers = player->getTowers();
    vector<Monster*>& monsters = pPlayer->getMonsters();
    int towersNum = towers.size();
    for(int i = 0; i < towersNum; ++i)
    {
        towers[i]->deductCoolingTime();
        auto it = monsters.begin();
        while(it != monsters.end())
        {
            if(towers[i]->inAttackRange((*it)))
            {
                towers[i]->attack((*it));
                pair<int, int>pos = getPosInWindow(towers[i]->getRow(), towers[i]->getCol());
                QList<QPair<int,int>>trajectory;
                int dx = btnWidth>>2,dy = btnHeight>>2;
                //qDebug() << pos.first << " " << pos.second << " " << (*it)->getX() << " " << (*it)->getY();
                getBulletTrajectory(pos.first+dx, pos.second+dy, (*it)->getX()+dx, (*it)->getY()+dy, trajectory);
                if(towers[i]->getKind() == BOTTLE)
                    bullets.append(trajectory);
                player->addMoney(5);    // Hit bonus.
                if(!(*it)->isAlive())
                {
                    checkAffixDrops();
                    player->addMoney((*it)->getBonus());  // Kill bonus.
                    delete *it;
                    it = monsters.erase(it);
                }
                else
                    ++it;
                // Each tower that isn't in rage mode can only attacks one monster.
                if(!towers[i]->inAreaDamageMode() || it == monsters.end())
                    break;
            }
            else
                ++it;
        }
        if(monsters.size() == 0)
            break;
    }
}


void PlayScene::timerEvent(QTimerEvent *event)
{
    if(gameover)
        return;
    // Generates new monsters.
    if(event->timerId() == timerid)
    {        
        bool genMonstersMode = pPlayer->InGenMonstersMode();
        if(genMonstersMode)
        {
            if(pPlayer->finished())
            {
                gameover = true;
                killAllTimers();
                showGameOver(":/items/win_bg.png");
                return;
            }
            wavesLabel->setText(QString::number(pPlayer->getCurWave())+ "/" + QString::number(WAVES) + " WAVES");
            pPlayer->genMonster();

        }
    }
    // Monsters moving.
    else if(event->timerId() == timerid1)
    {
        vector<Monster*>& monsters = pPlayer->getMonsters();
        for(auto it = monsters.begin(); it != monsters.end();)
        {
            if((*it)->isAlive())
            {
                if((*it)->inFrozen())
                {
                    ++it;
                    continue;
                }
                if((*it)->inBleeding())
                    (*it)->deductBlood();
                bool success = (*it)->move();
                if(!gameover)
                    gameover = !success;
                if(gameover)
                {
                    killAllTimers();
                    showGameOver(":/items/lose_bg.png");
                    return;
                    // click "restart"
                    // TO DO


                    // start a new game
                    // TO DO
                }
                ++it;
            }
            else
            {
                delete *it;
                it = monsters.erase(it);
            }
        }
        repaint();
    }
    // Check attack event.
    else if(event->timerId() == timerid2)
    {
        player->addMoney(20);
        monsterAttack();
        towerAttack();
        repaint();
    }
}


void PlayScene::showPrompt(int x, int y, QString text)
{
    prompt->setText(text);
    prompt->move(x, y);
    prompt->setVisible(true);
    QTimer::singleShot(1000, this, [=](){
        prompt->setVisible(false);
    });
}


inline void PlayScene::killAllTimers()
{
    killTimer(timerid);
    killTimer(timerid1);
    killTimer(timerid2);
}


void PlayScene::reset()
{
    deleteFigure();

    // if add this code when clicks a button in figure and then clicks back button, program will be crashed.
    //delete pre;

    pre = nullptr;
    for(int i = 0; i < selectTowerBtns.size(); ++i)
        delete selectTowerBtns[i];
    selectTowerBtns.clear();
    figure->reset();
    player->reset();
    pPlayer->reset();
    killAllTimers();
    gameover = false;
    affixInstallMode = false;
    affixNum.resize(AFFIX_KINDS, 0);
    int num = affixBtns.size();
    for(int i = 0; i < num; ++i)
        affixBtns[i]->setText(affixName(i+1)+QString::number(0));
}


// Load the paths of figure.
void PlayScene::loadFigure()
{
    //Add QLabel to display money.
    moneyLabel->setText(QString::number(player->getMoney()));
    wavesLabel->setText(QString::number(pPlayer->getCurWave()) + "/" + QString::number(WAVES) + " WAVES");

    int pathsNum = figure->getPathsNum();

    // Add picture flags on start and end positions.
    pair<int, int>end = figure->getEnd();
    if(end.first == -1)
        return;
    pair<int, int>pos = getPosInWindow(end.first, end.second);
    pair<int, int>start;
    addLabel(pos.first, pos.second,  btnWidth, btnHeight, ":/radish/radish17.png", 0.6);
    for(int i = 0; i < pathsNum; ++i)
    {
        start = figure->getStart(i);
        pos = getPosInWindow(start.first, start.second);
        addLabel(pos.first, pos.second, btnWidth, btnHeight, ":/items/startPoint.png", 0.8);
    }
    //figure->print();
}


bool PlayScene::prepare(QString& figurePath)
{    
    for(int i = 0; i < affixNum.size(); ++i)
    {
        affixNum[i] = 4;
        affixBtns[i]->setText(affixName(i+1)+QString::number(affixNum[i]));
    }
    if(figurePath.isEmpty())
        figure->genPaths();
    else
    {
        bool success = figure->genUserDefinedPath(figurePath);
        if(!success)
        {
            reset();
            return false;
        }

    }
    loadFigure();
    timerid = startTimer(4000);
    timerid1 = startTimer(400);
    timerid2 = startTimer(1000);
    return true;
}










