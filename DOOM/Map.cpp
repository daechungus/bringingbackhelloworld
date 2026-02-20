#include "Actor.h"
#include "Map.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Map;

//pass in smell distance so that goblin can take it later. 
Map::Map(int smellDistance) : m_smellDistance(smellDistance) {
    generateMap();
    m_player = new Player(1, 1, this); //this refers to a pointer to itself //this is probably the issue //delete this
    placePlayer();
    printMap();
}

//call destructor of map
Map::~Map() {
    delete m_player; //delete the player when we end the game

    for (auto monster : m_monsters) { //delete the vector of monsters when we end the game
        delete monster;
    }
    m_monsters.clear(); //clear the vector afterwards

    for (auto item : m_items) { //delete the vector of items affter end game
        delete item;
    }
    m_items.clear(); //clear vector after
}

//getters
int Map::getSmellDistance() const { return m_smellDistance; }
int Map::rows() const { return m_rowx; }
int Map::cols() const { return m_coly; }

//stores map as a template into another 2D array to keep couting
void Map::storeMap() {
    for (int row = 0; row < 18; row++) {
        for (int column = 0; column < 70; column++) {
            storegrid[row][column] = grid[row][column];
        }
    }

}

//generates a new map template every time it is called
void Map::generateMap() {
    //trivial 
    for (int row = 0; row < 18; row++) {
        for (int column = 0; column < 70; column++) {
            grid[row][column] = '#';
        }
    }
    //create a vector of rooms
    vector<Room> rooms; //did i need to delete this as well?

    int numberOfRooms = 100;
    for (int r = 0; r < numberOfRooms; r++) {
        Room room = { randInt(1, 8), randInt(1, 56), randInt(4, 8), randInt(9, 13) };
        if (noOverlap(room)) { //store each room if allowed
            createRoom(room);
            rooms.push_back(room);
        }
    }

    while (rooms.size() < 4) { //if we don't have more than 4 rooms, just keep running it.
        int numberOfRooms = randInt(50);
        for (int r = 0; r < numberOfRooms; r++) {
            Room room = { randInt(1, 8), randInt(1, 56), randInt(4, 8), randInt(9, 13) };
            if (noOverlap(room)) { //store each room if allowed
                createRoom(room);
                rooms.push_back(room);
            }
        }
    }


    //place a random staircase in a room 
    placeStaircase(); //I wanted staircase to be in a room, not a hallway so I called it before I conected my rooms

    //connect each room in my vector of rooms
    for (int i = 0; i < rooms.size() - 1; i++) {
        connectRooms(rooms[i], rooms[i + 1]);
    }

    rooms.clear();

    //store this template for later
    storeMap();

    //magic fangs of sleep, magic axes, and teleportation scrolls are never placed
    //maces, short swords, long swords, improvearmor, raisestrength, enhancehealth, increasedexterity scrolls

    //place random amount of items 
    for (int numberOfItems = randInt(2, 3); numberOfItems > 0; numberOfItems--) {
        GameObject* item = nullptr;

        int generateItemRandomly = randInt(7); //1/7 chance of each item being placed down
        switch (generateItemRandomly) {
        case 0:
            //place a mace
            item = new Mace(1, 1, this); //place a mace and push it into the vector of gameobejcts //delete these
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 1:
            //place a short sword
            item = new Shortsword(1, 1, this); //place a short sword and ....
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 2:
            //place a long sword
            item = new Longsword(1, 1, this); //place a long sowrd and ....
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 3:
            //place a improve armor
            item = new ScrollOfImproveArmor(1, 1, this); //place a armor scroll and ....
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 4:
            //place a raise strength
            item = new ScrollOfRaiseStrength(1, 1, this); //place a str scroll and. .. 
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 5:
            //place a enhance health
            item = new ScrollOfEnhanceHealth(1, 1, this); //place a hp scroll and ....
            m_items.push_back(item);
            placeObjects(item);
            break;

        case 6:
            //place a enhance dexterity
            item = new ScrollOfEnhanceDexterity(1, 1, this); //place a dex scroll and ...
            m_items.push_back(item);
            placeObjects(item);
            break;
        }
    }

    if (level == 0) {  //goblins, snakewomen
        for (int m = randInt(2, 5 * (level + 1) + 1); m > 0; m--) { //number of monsters on the floor

            Actor* monster = nullptr; //reset monster pointer
            //IF IT IS A vector of monsters, am i allowed to push back a snakewomen into that vector??


            int generateMonsterRandomly = randInt(2);

            if (generateMonsterRandomly == 0) {
                monster = new Snakewomen(1, 1, this);//make a snakewomen;
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }

            if (generateMonsterRandomly == 1) {
                //make a goblin
                monster = new Goblin(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
        }

    } //for each level, initialize the corresponding monsters and push them into the vector of monsters
    else if (level == 1) {         //goblins, snakewomen
        for (int m = randInt(2, 5 * (level + 1) + 1); m > 0; m--) { // . . . (exlpanation above)
            Actor* monster = nullptr; //reset monster pointer
            int generateMonsterRandomly = randInt(2);
            if (generateMonsterRandomly == 0) {
                //make a snakewomen;
                monster = new Snakewomen(1, 1, this);//make a snakewomen;
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
            if (generateMonsterRandomly == 1) {
                //make a goblin
                monster = new Goblin(1, 1, this);//make a goblin
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
        }
    } //if level == 1, ....
    else if (level == 2) {        //make goblins, snakewomen, bogeymen
        for (int m = randInt(2, 5 * (level + 1) + 1); m > 0; m--) { //trivial
            Actor* monster = nullptr; //reset monster pointer again
            int generateMonsterRandomly = randInt(3);
            if (generateMonsterRandomly == 0) {//HERE
                //make a snakewomen
                monster = new Snakewomen(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
            if (generateMonsterRandomly == 1) {
                //make a goblin
                monster = new Goblin(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
            if (generateMonsterRandomly == 2) { //this time make a bogeyman if levle >=2 
                //make a bogeymen
                monster = new Bogeyman(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
        }
    }
    else if (level == 3) {         //make dragons, goblins, snakewomen, bogeymen
        for (int m = randInt(2, 5 * (level + 1) + 1); m > 0; m--) { //make m appropriate monsters
            Actor* monster = nullptr; //reset monster ptr 
            int generateMonsterRandomly = randInt(4);
            if (generateMonsterRandomly == 0) {
                //make a snakewomen;
                monster = new Snakewomen(1, 1, this);//make a snakewomen;
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
            if (generateMonsterRandomly == 1) {
                //make a goblin
                monster = new Goblin(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
            if (generateMonsterRandomly == 2) {

                //make a bogeymen
                monster = new Bogeyman(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
            if (generateMonsterRandomly == 3) { //make a dragon now if level >= 3
                //make a dragon
                monster = new Dragon(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
        }
    }
    else if (level == 4) {
        GameObject* item = nullptr;
        item = new Goldenidol(1, 1, this); //create our golden idol
        m_items.push_back(item);
        placeObjects(item);
        setWinRow(item->row());
        setWinCol(item->col()); //set win condition coordinates 

        //make dragons, goblins, snakewomen, bogeymen
        for (int m = randInt(2, 5 * (level + 1) + 1); m > 0; m--) {
            Actor* monster = nullptr;
            int generateMonsterRandomly = randInt(4); //should be randInt(4) //remember to change back //changed it back
            if (generateMonsterRandomly == 0) {
                //make a snakewomen;
                monster = new Snakewomen(1, 1, this);//make a snakewomen;
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
            if (generateMonsterRandomly == 1) {
                //make a goblin
                monster = new Goblin(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);
            }
            if (generateMonsterRandomly == 2) {

                //make a bogeymen
                monster = new Bogeyman(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
            if (generateMonsterRandomly == 3) {
                //make a dragon
                monster = new Dragon(1, 1, this);
                m_monsters.push_back(monster);
                placeMonsters(monster);

            }
        }
    }
}
//players helper function to decode the user input into changing position
bool Map::determineNewPosition(int& r, int& c, char dir) const {
    switch (dir) {
    case ARROW_LEFT: //left
        c--;
        break;

    case ARROW_RIGHT: //right
        c++;
        break;

    case ARROW_UP: //up
        r--;
        break;

    case ARROW_DOWN: //down
        r++;
        break;

    default:
        break;
    }
    if (grid[r][c] != '#') { // add condition
        return true;
    }
    return false;
}

//getters for player pointer, monster pointer, gameobject pointer, and pointers to vector of monsters and gameobjects
Player* Map::player() const { return m_player; }
Actor* Map::monster(int index) const { return m_monsters[index]; }
GameObject* Map::item(int index) const { return m_items[index]; }
vector<Actor*>& Map::getMonsterVector() { return m_monsters; }
vector<GameObject*>& Map::getItemVector() { return m_items; }

//helper function to randomly place player on valid spot
void Map::placePlayer() {
    int row = randInt(1, 17);
    int col = randInt(1, 69);
    while (!placeDown(row, col)) {
        row = randInt(1, 17);
        col = randInt(1, 69);
    }
    m_player->setRow(row);
    m_player->setCol(col);
    grid[row][col] = m_player->description();
}

void Map::placeObjects(GameObject* i) { //helper function to randomly place gameobject on a valid spot
    if (!itemPlaceDown(i->row(), i->col())) {
        int row = randInt(1, 17);
        int col = randInt(1, 69);

        while (!itemPlaceDown(row, col)) {
            row = randInt(1, 17);
            col = randInt(1, 69);
        }
        i->setRow(row);
        i->setCol(col);
        grid[row][col] = i->description();
    }
    else
        itemPlaceDown(i->row(), i->col());
}

void Map::placeMonsters(Actor* m) {
    int row = randInt(1, 17);
    int col = randInt(1, 69);

    while (!placeDown(row, col)) {
        row = randInt(1, 17);
        col = randInt(1, 69);
    }

    m->setRow(row);
    m->setCol(col);

    grid[row][col] = m->description();

} //helper function to randomly place mosnter on valid spot

void Map::endGame() { //called after picking up golden idol is this even needed??
    delete m_player;

    for (auto index : m_monsters) {
        delete index;
    }
    m_monsters.clear(); //vector of monsters destructor

    for (auto index : m_items) {
        delete index;
    }
    m_items.clear();
}


void Map::nextLevel() { //call next level everytime we descend, clearing items and monsters on that level and proceeding onwards. 
    //call destructors
    level++;
    for (auto index : m_monsters) {
        delete index;
    }
    m_monsters.clear(); //vector of monsters destructor

    for (auto index : m_items) {
        delete index;
    }
    m_items.clear();

    generateMap(); //make a new map template
    placePlayer(); //place the player onto new map
}

//make a specific exit coord

void Map::placeStaircase() { //places staircase randomly.
    int row = randInt(1, 17);
    int col = randInt(1, 69);
    while (!placeDown(row, col)) {
        row = randInt(1, 17);
        col = randInt(1, 69);
    }

    if (level != 4) {
        setExitRow(row); //sets exit positions
        setExitCol(col);
        grid[row][col] = '>';
    }
}

//getters and setters for staircase and golden idol positions 
int Map::exitRow() const { return exitrow; }
int Map::exitCol() const { return exitcol; }
void Map::setExitRow(int coord) { exitrow = coord; }
void Map::setExitCol(int coord) { exitcol = coord; }
int Map::winRow() const { return winrow; }
int Map::winCol() const { return wincol; }
void Map::setWinRow(int coord) { winrow = coord; }
void Map::setWinCol(int coord) { wincol = coord; }

char Map::getGrid(int row, int col) const { return grid[row][col]; } //getter for my grid coords

//helper function to create a room
void Map::createRoom(const Room& room) {
    for (int i = room.rowx; i < room.rowx + room.height; i++) {
        for (int j = room.coly; j < room.coly + room.width; j++) {
            grid[i][j] = ' ';
        }
    }
}
//checks if each room can be created without passing over another room
bool Map::noOverlap(const Room& room) const {
    for (int i = max(0, room.rowx - 1); i < min(17, room.rowx + room.height + 1); i++) { //add the +1 -1 or else a room can be right next to another 
        for (int j = max(0, room.coly - 1); j < min(70, room.coly + room.width + 1); j++) {
            if (grid[i][j] != '#') {
                return false;
            }
        }
    }
    return true;
}
void Map::drawHorizontalCorridor(int r, int c1, int c2) { //vertical corridor actually
    if (c2 < c1) swap(c1, c2); //swapes to remove else statement
    for (int c = c1; c <= c2; c++) {
        if (grid[r][c] == '#') grid[r][c] = ' ';
    }
} //draws a horizontal corridor from a random coord on the first room to the second room


void Map::drawVerticalCorridor(int c, int r1, int r2) { //horizontal corridor actually
    if (r2 < r1) swap(r1, r2); //swaps to remove else statement
    for (int x = r1; x <= r2; x++) {
        if (grid[x][c] == '#') grid[x][c] = ' ';
    }
} //draws a vertical corridor from ... . . .  . .

void Map::connectRooms(const Room& room1, const Room& room2) {
    int y1 = randInt(room1.coly + 1, room1.coly + room1.width - 1); //get random coords for row, col of room 1 to room 2
    int x1 = randInt(room1.rowx + 1, room1.rowx + room1.height - 1);
    int y2 = randInt(room2.coly + 1, room2.coly + room2.width - 1);
    int x2 = randInt(room2.rowx + 1, room2.rowx + room2.height - 1);


    //get top left row1 and col1 coord
    //get top left row2 and col2 coord

    //if (randInt(0, 1) == 1) {
    //    drawHorizontalCorridor(row1, col1, col2) 

    //}

    if (randInt(0, 1) == 1) { //adds randomness so it doesn't always do hor to vert or vert to hor order
        drawHorizontalCorridor(x1, y1, y2);
        drawVerticalCorridor(y2, x1, x2);
    }
    else {
        drawVerticalCorridor(y2, x1, x2);
        drawHorizontalCorridor(x1, y1, y2);
    }
} //connects the two rooms by drawing corridors 

void Map::printMap() {
    for (int row = 0; row < 18; row++) {
        for (int col = 0; col < 70; col++) {
            if (storegrid[row][col] == '#')
                grid[row][col] = '#';
            else
                grid[row][col] = ' ';
        }
    }
    //updates the grid to be the stored template


    updateMap(); //calls update map

    for (int row = 0; row < 18; row++) {
        for (int col = 0; col < 70; col++) {
            cout << grid[row][col];
        }
        cout << endl;
    }
    //prints the template 

    //just couts the status bar 
    cout << "Level: " << level << ", Hit points: " << m_player->getHealth() << ", Armor: " << m_player->getArmor() << ", Strength: " << m_player->getStrength() << ", Dexterity: " << m_player->getDexterity() << endl << endl;

    //gets the players output string
    cout << m_player->getOutputString();
    m_player->setOutputString("");//resets output string 
    //runs through the vector of monsters and outputs each output string
    for (auto index = m_monsters.begin(); index != m_monsters.end(); index++) {
        Actor* monster = *index;
        cout << monster->getOutputString();
        monster->setOutputString("");
    }
}

void Map::updateMap() {

    for (int row = 0; row < 18; row++) {
        for (int column = 0; column < 70; column++) {
            grid[row][column] = storegrid[row][column]; //updates the grid we output to the template we stored
        }
    }

    for (auto index = m_items.begin(); index != m_items.end(); index++) { //updates each game object on the map to show up on the map
        GameObject* item = *index;
        int row = item->row();
        int col = item->col();
        grid[row][col] = item->description();
    }

    int r = m_player->row();
    int c = m_player->col();
    grid[r][c] = m_player->description(); //updates the players position 

    for (auto index = m_monsters.begin(); index != m_monsters.end(); index++) { //updates each monsters position
        Actor* monster = *index;
        int row = monster->row();
        int col = monster->col();
        grid[row][col] = monster->description(); //NO MORE ERROR NOW 
    }

}

bool Map::placeDown(int row, int col) const { //basic actor helper function to check if it can be placed down
    if (grid[row][col] != '#' && grid[row][col] != 'G' && grid[row][col] != 'B' && grid[row][col] != 'S' && grid[row][col] != 'D' && grid[row][col] != '@') //suppoed to check if there is a monster there or not
        return true;

    return false;
}

bool Map::goblinPlaceDown(int row, int col) const { //should remove the player part or else it will never read i
    if (grid[row][col] != '#' && grid[row][col] != 'G' && grid[row][col] != 'B' && grid[row][col] != 'S' && grid[row][col] != 'D') //suppoed to check if there is a monster there or not
        return true;

    return false;
}
bool Map::itemPlaceDown(int row, int col) const { //basic gameobject helper function to check if it can be placed down.
    if (grid[row][col] != '#' && grid[row][col] != '?' && grid[row][col] != ')' && grid[row][col] != '>' && grid[row][col] != '&') { return true; }
    return false;
}



