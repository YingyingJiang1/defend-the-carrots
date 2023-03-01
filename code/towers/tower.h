#ifndef TOWER_H
#define TOWER_H

#include "qpushbutton.h"
#include "towerAttrs.h"
#include "monster.h"

#include <QPushButton>
#include <QList>

extern const int numOfTowerKinds;

// TO DO:
// Set function and attributes for each tower.

typedef enum TowerKind{
    BOTTLE = 0,
    SHIT,
    FAN
}TowerKind;

class Tower
{
private:
    // coordinate of the tower
    int row, col;
    int cost;
    int blood;


    int attRange;
    /*
     * if attRange = 2 and the tower is '*',
     * then locations marked by '#' are locations can be attacked.
     * -------#####-----------
     * -------#####-----------
     * -------##*##-----------
     * -------#####-----------
     * -------#####-----------
    */
    int damage;
    vector<int>affixes;
    vector<int>cooling;
    QList<QPushButton*> affixBtns;

public:
    explicit Tower(int _row, int _col, int _cost, int _blood, int _attRange, int _damage);
    Tower(){}
    ~Tower();
    int attack(Monster* target);
    string getAffixName(int i);
    void damaged(int damageSuffered);
    void deductCoolingTime();
    bool emptyAffix(int i){return affixes[i] == 0;}
    bool inAttackRange(Monster* target);
    bool inCooling(int i);
    int installAffix(int affix, int slot);
    bool isAlive();
    bool inAreaDamageMode();
    int getBlood(){return blood;}
    int getRow(){return row;}
    int getCol(){return col;}
    int getDamage(){return damage;}
    void setAffixBtn(QPushButton* btn, int i){affixBtns[i] = btn;}
    // Uninstall affix and return NO. of the affix uninstalled.
    int uninstallAffix(int i);
    virtual TowerKind getKind() = 0;
};


/*
 * Bottles are deployed outside the path. Each bottle deployed costs 100 gold.
 * Blood of each bottle is ??.
 * Default value of 'attRange' and 'damage' are 2, 1 repectively .
*/
class Bottle:public Tower
{

public:
    Bottle(int _row, int _col);
    Bottle(){}
    virtual TowerKind getKind(){return BOTTLE;}
};


class Shit:public Tower
{

public:
    Shit(int _row, int _col);
    Shit(){}
    virtual TowerKind getKind(){return SHIT;}

};



/*
 * Fans are deployed on the path, each fan deployed costs 160 gold.
 * Blood of each fan is ??.
 *Default value of 'attRange' and 'damage' are 1, 1 respectively.
 *Fans can block the monster's progress
*/
class Fan:public Tower
{

public:
    Fan(int _row, int _col);
    Fan(){}
    virtual TowerKind getKind(){return FAN;}

};



inline Tower::~Tower()
{
    for(int i = 0; i < affixBtns.size(); ++i)
        delete affixBtns[i];
}



inline void Tower::damaged(int damageSuffered)
{
    blood -= damageSuffered;
    if(blood < 0)
        blood = 0;
}

inline void setBtnTextColor(QColor color, QPushButton* btn)
{
    QPalette palette;
    palette.setColor(QPalette::ButtonText, color);
    btn->setPalette(palette);
}

inline void Tower::deductCoolingTime()
{
    int num = cooling.size();
    for(int i = 0; i < num; ++i)
        if(cooling[i] > 0)
        {
            --cooling[i];
            if(cooling[i] > 0)
                setBtnTextColor(Qt::gray, affixBtns[i]);
            else
                setBtnTextColor(Qt::black, affixBtns[i]);
        }
}

inline bool Tower::inCooling(int i)
{
    if(cooling[i] == 0)
        return false;
    --cooling[i];
    return true;
}

inline bool Tower::isAlive()
{
    if(blood > 0)
        return true;
    return false;
}

inline bool Tower::inAreaDamageMode()
{
    int size = affixes.size();
    for(int i = 0; i < size; ++i)
        if(cooling[i] == 0 && affixes[i] == AREA_DAMAGE_ATTR)
            return true;
    return false;
}

inline Bottle::Bottle(int _row, int _col)
    :Tower(_row, _col, BOTTLE_COST, BOTTLE_BLOOD, BOTTLE_ATT_RANGE, BOTTLE_DAMAGE)
{}

inline Fan::Fan(int _row, int _col)
    :Tower(_row, _col, FAN_COST, FAN_BLOOD, FAN_ATT_RANGE, FAN_DAMAGE)
{}

inline Shit::Shit(int _row, int _col)
    :Tower(_row, _col, SHIT_COST, SHIT_BLOOD, SHIT_ATT_RANGE, SHIT_DAMAGE)
{}

inline QString affixName(int affix)
{
    if(affix == RAGE_ATTR)
        return "狂暴";
    if(affix == FROST_ATTR)
        return "冰系";
    if(affix == AREA_DAMAGE_ATTR)
        return "群伤";
    if(affix == BLEEDING_ATTR)
        return "放血";
    return "空";
}


#endif // TOWER_H
