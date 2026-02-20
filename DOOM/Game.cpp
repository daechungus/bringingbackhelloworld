// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance) : smellDistance(goblinSmellDistance) {
    //return goblinSmellDistance;
    ////just figure this out 
    m_map = new Map(goblinSmellDistance);
}

Game::~Game() {
    delete m_map;
}

void Game::play() {

    //Map* m_map = new Map(); //initialize a new map
    Player* p = m_map->player(); // initialize a pointer to the maps player

    char command = ' ';
    char wieldWeapon = ' ';
    char readScroll = ' ';

    while (command != 'q') {
        /*
        if (command == 'q') {
            game = false;
        }
        else {
         */
        command = getCharacter();

        switch (command) {
            //call player's position to be set equal to itself or just don't move at all.
            //monster's move.
        //my simple case to check levels, remove this later.
        //case '1': //cheat tester functions
        //    m_map->nextLevel();
        //    break;
        //case '2':
        //    p->setSleep(10);
        //    break;
        //case '3':
        //    p->increaseArmor(100);
        //    p->increaseDexterity(100);
        //    p->increaseMaxHealth(100);
        //    p->increaseStrength(100);
        //    //include a wield weapon
        //    break;
        //case '4':
        //    p->getWeapon();
        //    break;
        case 'i': //open inventory
            clearScreen();
            p->printInventory();
            getCharacter();
            //openInventory = true;
            break;
        case 'w': //wield weapon
            clearScreen();
            p->printInventory();
            wieldWeapon = getCharacter();
            p->switchWeapons(wieldWeapon);
            //cerr output properly
            break;
        case 'r': //read scroll
            clearScreen();
            p->printInventory();
            readScroll = getCharacter();
            p->useScroll(readScroll);
            break;
        case '>': //descend deeper
            if (p->row() == m_map->exitRow() && p->col() == m_map->exitCol())
                m_map->nextLevel();
            break;
        case 'g': //pick up item 
            p->addItem();
            if (p->getWin()) {
                cout << p->getOutputString();
                char winCommand = ' ';
                do {
                    winCommand = getCharacter();

                } while (winCommand != 'q');
                //m_map->endGame(); //endgame scenario 3
                cout << "Press any key to close the window . . . " << endl;
                getCharacter();
                command = 'q';
                break;
            }
            break;
        case ARROW_LEFT: //arrow keys 
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
            p->move(command);
            p->getOutputString();
            break;
        case 'c': //cheat
            if (p->getSleep() == 0) {
                p->setHealth(50);
                p->setStrength(9);
            }
            else
                p->setSleep(p->getSleep() - 1);
            break;
        case 'q': //quit anytime
            if (p->getSleep() > 0) {
                p->setSleep(p->getSleep() - 1);
            }

            //p->setOutputString("Press any key to close the window . . . ");
            clearScreen();
            m_map->printMap();
            //m_map->endGame(); //endgame scenario 2
            cout << "Press any key to close the window . . . " << endl;
            getCharacter();
            command = 'q';
            break;
        default:
            break;
        }

        m_map->updateMap();

        //check if any monsters are dead, drop item and then delete them
        if (command != 'q') {
            for (auto monster : m_map->getMonsterVector()) {
                monster->move();
                m_map->updateMap();
                monster->getOutputString();

                if (p->isPassedOut()) {  //call endgame and freeze jgame. endgame scenario 1
                    clearScreen();
                    m_map->printMap();
                    cout << "Press q to exit game." << endl;
                    char exitCommand = ' ';
                    do {
                        exitCommand = getCharacter();

                    } while (exitCommand != 'q');
                    //m_map->endGame();
                    cout << "Press any key to close window . . . " << endl;
                    getCharacter();
                    command = 'q';
                    break;

                }

            }
        }

        if (command != 'q') { //otherwise I would change the output of the endgame sequence
            clearScreen();
            m_map->printMap();
        }
    }
}


// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
