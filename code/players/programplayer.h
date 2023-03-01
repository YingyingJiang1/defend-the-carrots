#ifndef PROGRAMPLAYER_H
#define PROGRAMPLAYER_H
#include "monster.h"
#include <vector>



#define WAVES 1


class ProgramPlayer
{

private:
    int monsterNO;
    int wave;  // when 'wave' equals to 0, stop to generate animals.
    int width, height;
    int remainingMonsters;  // Remaining monsters to be generated.
    int genMonstersMode;
    Figure* figure;
    vector<Monster*>monsters;

    Monster *genSpecificMonster(int pathNO, MonsterKind kind);
    void getGenInfo(int& pathNO, MonsterKind& kind);
    int getMonstersNum();
public:
    explicit ProgramPlayer(){}
    ProgramPlayer(Figure* _figure, int _width, int _height);
    ~ProgramPlayer();
    void cleanDeadMonsters();
    vector<Monster*>::const_iterator deleteMonster(vector<Monster*>::const_iterator it){delete *it; return monsters.erase(it);}
    bool finished(){return wave > WAVES;}
    int getCurWave(){return wave;}
    void genMonster();
    bool noMonstersAlive();
    vector<Monster*>& getMonsters(){return monsters;}
    bool InGenMonstersMode();
    void reset();

};

inline int ProgramPlayer::getMonstersNum()
{
    if(wave == 1)
        return 8;
}


inline bool ProgramPlayer::noMonstersAlive()
{
    if(monsters.size() == 0)
        return true;
    return false;
}



#endif // PROGRAMPLAYER_H
