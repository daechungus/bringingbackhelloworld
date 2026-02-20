#ifndef GAME_INCLUDED
#define GAME_INCLUDED


#include "utilities.h"
#include "Map.h"

#include <iostream>
#include <vector>
using namespace std;

class Game {

private:
    Map* m_map;
    int smellDistance;

public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();
};

#endif // GAME_INCLUDED
