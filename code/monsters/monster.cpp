#include "monster.h"
#include "tower.h"

// The number of pixels moved each time a timeout event occurs are speed*SPEED_UNIT
#define SPEED_UNIT 5

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

const int numOfMonsterKinds = 3;

const int Monster::bonus = 14;

const QString StarMonster::imagePath = ":/monsters/land_star01.png";

const QString PinkMonster::imagePath = ":/monsters/land_pink01.png";

const QString FlyBlueMonster::imagePath = ":/monsters/fly_blue01.png";
const int FlyBlueMonster::bonus = 20;



Monster::Monster(Figure * _figure, int _pathNO, int _width, int _height,
                 int _id, int _blood, int _speed, int _attRange, int _damage)
{
    figure = _figure;
    pathNO = _pathNO;
    curIndex = 0;
    width = _width;
    height = _height;
    id = _id;
    getPixelLoc(x, y, curIndex);
    blood = _blood;
    speed = _speed;
    attRange = _attRange;
    damage = _damage;
    stopMoving = 0;
    bleeding = 0;
    coolingTime = 0;
    direction = getMoveDirection();
    for(int i = 0; i < M_AFFIX_SLOTS; ++i)
    {
        int number = myRand(10);
        if(number == 1 || number == 2)
            affixes.push_back(number);
        else
            affixes.push_back(0);
    }
}



int Monster::getMoveDirection()
{
    if(curIndex + 1 >= figure->getPathLength(pathNO))
        return 0;
    int nx, ny;
    getPixelLoc(nx, ny, curIndex+1);
    if(nx < x)
        return LEFT;
    if(nx > x)
        return RIGHT;
    if(ny < y)
        return DOWN;
    if(ny > y)
        return UP;
}


bool Monster::inAttackRange(Tower *target)
{
    if(!target->isAlive())
        return false;

    int tRow = target->getRow(), tCol = target->getCol();
    pair<int, int> monsterLoc = figure->getLoc(pathNO, curIndex);
    int row = monsterLoc.first, col = monsterLoc.second;

    if((tRow == row && tCol >= col - attRange && tCol <= col + attRange)
        || (tCol == col && tRow >= row - attRange && tRow <= row + attRange))
        return true;

    return false;
}


bool Monster::attack(Tower *target)
{
    target->damaged(damage);

}


inline int min(int x, int y)
{
    if(x < y)
        return x;
    else
        return y;
}


inline int max(int x, int y)
{
    if(x > y)
        return x;
    else
        return y;
}


bool Monster::move()
{
    int curx, cury;
    getPixelLoc(curx, cury, curIndex);
    int dis = speed * SPEED_UNIT;
    bool noblink = true;

    for(int i = 0; i < affixes.size(); ++i)
    {
        // If the monster has agility attribute then it moves twice as fast.
        if(affixes[i] == AGILITY_ATTR)
            dis <<= 1;
        else if(affixes[i] == BLINK_ATTR)
            noblink = false;
    }

    // The monster should move to next location in figure.
    if(x == curx && y == cury)
    {
        // Reaches the end.
        if(curIndex + 1 >= figure->getPathLength(pathNO))
            return false;

        // Next location is not empty and the monster hasn't blink attribute, stops to move.
        bool block = figure->notEmptyLocation(figure->getLoc(pathNO, curIndex + 1));
        if(block && (noblink || coolingTime > 0))
            return true;
        //Unleashes blink skill.
        if(block && !noblink && coolingTime == 0)
            coolingTime = 50;
        ++curIndex;
        getPixelLoc(curx, cury, curIndex);

    }
    // Moving up or down
    if(x == curx)
    {
        // Moving direciton: up
        if(y < cury)
        {
            y = min(y + dis, cury);
        }
        // Moving direction: down
        else
        {
            y = max(y - dis, cury);
        }
    }
    // Moving left or right.
    else
    {
        // Moving direction: right
        if(x < curx)
        {
            x = min(x + dis, curx);
        }
        // Moving direction: left
        else
        {
            x = max(x - dis, curx);
        }
    }
    return true;
}



