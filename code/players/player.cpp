#include "player.h"
#include <iostream>
using namespace std;
#include <QDebug>

Player::Player(int _money)
{
    money = _money;
}

Player::~Player()
{
    int size = 0;
    for(int i = 0; i < size; ++i)
        delete towers[i];
}

void Player::cleanDeadTowers()
{
    for(auto it = towers.begin(); it != towers.end();)
    {
        if((*it)->isAlive())
            ++it;
        else
        {
            delete *it;
            it = towers.erase(it);
        }
    }
}

Tower* Player::deploy(Figure* figure, pair<int, int> location, TowerKind kind)
{
    if(figure->notEmptyLocation(location))
        return nullptr;
    Tower* tower;
    int row = location.first, col = location.second;        
    switch(kind)
    {
    case BOTTLE:
    {
        if(money < BOTTLE_COST)
            return nullptr;
        tower = new Bottle(row, col);
        money -= BOTTLE_COST;
        break;
    }
    case SHIT:
    {
        if(money < SHIT_COST)
            return nullptr;
        tower = new Shit(row, col);
        money -= SHIT_COST;
        break;
    }
    case FAN:
    {
        if(money < FAN_COST)
            return nullptr;
        tower = new Fan(row, col);
        money -= FAN_COST;
        break;
    }
    default:
        cout << "Error type of tower!" << endl;
        break;
    }
    towers.push_back(tower);
    figure->setLocation(location.first, location.second, true);
    return tower;
}

void Player::reset()
{
    money = 180;
    for(int i = 0; i < towers.size(); ++i)
        delete towers[i];
    towers.clear();
}




