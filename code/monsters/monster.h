#ifndef MONSTER_H
#define MONSTER_H

#include "monsterAttrs.h"
#include "figure.h"



class Tower;

extern int extraHeight;
extern const int numOfMonsterKinds;

typedef enum MonsterKind
{
    STAR_MONSTER = 0,
    PINK_MONSTER,
    FLY_BLUE_MONSTER
}MonsterKind;

class Monster
{

protected:
    Figure* figure;
    /* 'pathNO' determines which path the monster in,
    *'curIndex' determines where the monster on the path.
    */
    int pathNO, curIndex;
    int x, y; // The starting pixel on the screen.
    int width, height;  // The range of pixels occupied by each point on the path.
    int id;
    int blood;      // if 'blood' becomes 0, then the animal dies.
    int speed;  // speed of movement
    int direction;
    int stopMoving; // When this value is greater than 0, the monster stops moving.
    int bleeding;   // When this value is greater than 0, the monster is bleeding.
    static const int bonus; // The bonus tower gets when hitting the monster, following are the same.
    /*
     * if attRange = 2 and the animal is '*',
     * then locations marked by '#' are locations can be attacked.
     * ---------#-------------
     * ---------#-------------
     * -------##*##-----------
     * ---------#-------------
     * ---------#-------------
    */
    int attRange;
    int damage; // if a tower is attacked by a boar, then deduct 'damage' blood from tower.
    vector<int>affixes;
    int coolingTime;    // cooling time for blink skill
public:
    explicit Monster(){}
    Monster(Figure* figure, int _pathNO, int _width, int _height, int _id, int _blood,
            int _speed, int _attRange, int _damage);

    string affixName(int affix);
    void damaged(int damageSuffered, int _stopMoving, int _bleeding);
    int deductBlood();
    void deductCoolingTime();
    string getAffixName(int i);
    int getBlood(){return blood;}
    int getId(){return id;}
    virtual int getBonus(){return bonus;}
    // figure:(row, col) --> screen: (x, y) ((x, y) is the starting pixel.
    void getPixelLoc(int& x, int& y, int index);
    int getRow(){return figure->getLoc(pathNO, curIndex).first;}
    int getCol(){return figure->getLoc(pathNO, curIndex).second;}
    int getMoveDirection();
    int getX(){return x;}
    int getY(){return y;}
    bool inAttackRange(Tower* target);
    bool inBleeding();
    bool inFrozen();
    bool attack(Tower* target);
    //bool inAttackRange(Tower* tower);
    int isAlive(){return blood;}
    // If the monster reaches the end, then return false.
    bool move();    
    virtual MonsterKind getKind() = 0;
};


class StarMonster:public Monster
{
private:
    static const QString imagePath ;
public:
    StarMonster(Figure* figure, int _pathNO, int _width, int _height, int _id)
        : Monster(figure, _pathNO, _width, _height, _id,
                  STAR_MONSTER_BLOOD, STAR_MONSTER_SPEED, STAR_MONSTER_ATT_RANGE, STAR_MONSTER_DAMAGE)
    {}

    MonsterKind getKind(){return STAR_MONSTER;}

};


class PinkMonster: public Monster
{
private:
    static const QString imagePath;
public:
    PinkMonster(Figure* figure, int _pathNO, int _width, int _height, int _id)
        : Monster(figure, _pathNO, _width, _height, _id,
                  STAR_MONSTER_BLOOD, STAR_MONSTER_SPEED, STAR_MONSTER_ATT_RANGE, STAR_MONSTER_DAMAGE)
    {}

    MonsterKind getKind(){return PINK_MONSTER;}

    static const string &getFilename();
};


class FlyBlueMonster: public Monster
{
private:
    static const QString imagePath;
    static const int bonus;
public:
    FlyBlueMonster(Figure* figure, int _pathNO, int _width, int _height, int _id)
        : Monster(figure, _pathNO, _width, _height, _id,
                  STAR_MONSTER_BLOOD, STAR_MONSTER_SPEED, STAR_MONSTER_ATT_RANGE, STAR_MONSTER_DAMAGE)
    {}

    MonsterKind getKind(){return FLY_BLUE_MONSTER;}
    virtual int getBonus(){return bonus;}

};



inline string Monster::getAffixName(int i)
{
    return affixName(affixes[i]);
}


inline string Monster::affixName(int affix)
{
    if(affix == BLINK_ATTR)
        return "闪现的";
    else if(affix == AGILITY_ATTR)
        return "神速的";
    return "";
}

inline void Monster::damaged(int damageSuffered, int _stopMoving, int _bleeding)
{
    stopMoving = _stopMoving;
    bleeding = _bleeding;
    if(blood > damageSuffered)
        blood -= damageSuffered;
    else
        blood = 0;
}

inline int Monster::deductBlood()
{
    if(blood > 0)
        --blood;
    return blood;
}

inline void Monster::deductCoolingTime()
{
    if(coolingTime > 0)
        --coolingTime;
}

inline void Monster::getPixelLoc(int &x, int &y, int index)
{
    pair<int, int> loc = figure->getLoc(pathNO, index);
    x = width * loc.second;
    y = (extraHeight>>1) + loc.first * height;
}

inline bool Monster::inBleeding()
{
    if(bleeding > 0)
    {
        --bleeding;
        // If 'bleeding' is greater than 0 then deducting one blood for every two moves.
        if((bleeding & 0x1) == 0)
            return true;
        return false;
    }
    return false;
}

inline bool Monster::inFrozen()
{
    if(stopMoving > 0)
    {
        --stopMoving;
        return true;
    }
    return false;
}







#endif // MONSTER_H
