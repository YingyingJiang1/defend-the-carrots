#include "tower.h"

const int numOfTowerKinds = 3;

Tower::Tower(int _row, int _col, int _cost, int _blood, int _attRange, int _damage)
{
    row = _row;
    col = _col;
    cost = _cost;
    blood = _blood;
    attRange = _attRange;
    damage = _damage;
    affixes.resize(AFFIX_SLOTS, 0);
    cooling.resize(AFFIX_SLOTS, 0);
    affixBtns.resize(AFFIX_SLOTS);
}



int Tower::attack(Monster *target)
{
    int _damage = damage;
    int stopMoving = 0;
    int bleeding = 0;
    int size = affixes.size();
    for(int i = 0; i < size; ++i)
    {
        if(cooling[i] > 0)
            continue;
        if(affixes[i] == RAGE_ATTR)
        {
            _damage = damage << 1;
            cooling[i] = COOLING_TIME;
        }
        else if(affixes[i] == FROST_ATTR)
        {
            stopMoving = STOP_MOVING;
            cooling[i] = COOLING_TIME;
        }
        else if(affixes[i] == BLEEDING_ATTR)
        {
            bleeding = BLEEDING;
            cooling[i] = COOLING_TIME;
        }
    }
    target->damaged(_damage, stopMoving, bleeding);
    return target->getBonus();
}


bool Tower::inAttackRange(Monster *target)
{
    if(!target->isAlive())
        return false;

    int tRow = target->getRow(), tCol = target->getCol();

    if(tRow >= row - attRange && tRow <= row + attRange
    && tCol >= col - attRange && tCol <= col + attRange)
        return true;

    return false;
}

// When the function is called, the slot must be empty.
int Tower::installAffix(int affix, int slot)
{
    int num = affixes.size();
    if(getKind() == SHIT && affix == BLEEDING_ATTR)
        return 0;
    else if(getKind() == BOTTLE && affix != BLEEDING_ATTR)
        return 1;
    for(int i = 0; i < num; ++i)
        if(affixes[i] == affix)
            return 2;
    affixes[slot] = affix;
    cooling[slot] = 0;
    affixBtns[slot]->setText("词缀" + QString::number(slot+1) + ":" + affixName(affix));
    return 3;
}

int Tower::uninstallAffix(int i)
{
    int affixNO = affixes[i];
    if(affixes[i] != 0)
    {
        affixBtns[i]->setText("词缀"+QString::number(i+1)+":空");
        affixes[i] = 0;
        cooling[i] = 0;
    }
    return affixNO;
}




