#include "programplayer.h"


ProgramPlayer::ProgramPlayer(Figure* _figure, int _width, int _height)
{
    monsterNO = 1;
    wave = 1;
    width = _width;
    height = _height;
    figure = _figure;      
    genMonstersMode = true;
    remainingMonsters = 0;
}


ProgramPlayer::~ProgramPlayer()
{
    int size = monsters.size();
    for(int i = 0; i < size; ++i)
        delete monsters[i];
}


void ProgramPlayer::cleanDeadMonsters()
{
    for(auto it = monsters.begin(); it != monsters.end();)
    {
        if((*it)->isAlive())
        {
            ++it;
        }
        else
        {
            delete *it;
            it = monsters.erase(it);

        }
    }
}


bool ProgramPlayer::InGenMonstersMode()
{
    if(remainingMonsters == 0 && monsters.size() > 0)
        genMonstersMode = false;
    else
    {
        genMonstersMode = true;
        if(remainingMonsters == 0)
            remainingMonsters = getMonstersNum();
    }

    return genMonstersMode;
}


void ProgramPlayer::reset()
{

    int size = monsters.size();
    for(int i = 0; i < size; ++i)
        delete monsters[i];
    monsters.clear();
    wave = 1;
    monsterNO = 1;
    remainingMonsters = 0;
    genMonstersMode = true;
}


void ProgramPlayer::getGenInfo(int &pathNO, MonsterKind &kind)
{
    if(wave == 1)
    {
        pathNO = 0;
        kind = (MonsterKind)myRand(3);
    }
    else
    {

    }
}


void ProgramPlayer::genMonster()
{    
    int pathNO;
    MonsterKind kind;
    getGenInfo(pathNO, kind);
    Monster* monster = genSpecificMonster(pathNO, kind);
    if(monster)
        monsters.push_back(monster);
    --remainingMonsters;
    if(remainingMonsters == 0)
        ++wave;
}



Monster *ProgramPlayer::genSpecificMonster(int pathNO, MonsterKind kind)
{
    Monster* monster = nullptr;
    try{
        switch(kind)
        {
        case STAR_MONSTER:
        {
            monster = new StarMonster(figure, pathNO, width, height, monsterNO++);
            return monster;
        }
        case PINK_MONSTER:
        {
            monster = new PinkMonster(figure, pathNO, width, height, monsterNO++);
            return monster;
        }
        case FLY_BLUE_MONSTER:
        {
            monster = new FlyBlueMonster(figure, pathNO, width, height, monsterNO++);
            return monster;
        }
        default:
            break;
        }
    }
    catch (const bad_alloc& e)
    {
        return nullptr;
    }
    return monster;
}
