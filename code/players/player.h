#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
using namespace std;
#include "tower.h"
#include "figure.h"

class Player
{
private:
    int money;  // balance of the player
    vector<Tower*> towers;
public:
    Player(int _money = 180);
    ~Player();
    void addMoney(int bonus){money += bonus;}
    vector<Tower*>::const_iterator deleteTower(vector<Tower*>::const_iterator it){delete *it; return towers.erase(it);}
    void cleanDeadTowers();
    Tower* deploy(Figure* figure, pair<int, int>location, TowerKind kind);
    vector<Tower*>& getTowers(){return towers;}
    int getMoney(){return money;}
    void reset();
    //deployTower()
};


#endif // PLAYER_H
