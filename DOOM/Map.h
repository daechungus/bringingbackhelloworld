#ifndef MAP_H
#define MAP_H

#include "utilities.h"
#include "Actor.h"
#include "Map.h"
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

class Actor;
class Player;
class GameObject;

using namespace std;

class Map {

private: //protected
	struct Room { int rowx, coly, height, width; }; //x is rows, y is column

	char grid[18][70];
	char storegrid[18][70];

	Player* m_player;
	//vector<Room*> m_rooms; //?? 
	vector<GameObject*> m_items;
	vector<Actor*> m_monsters; //make this a vector or array of monster pointers. initialize a new monster object and push it into the vector
	int m_rowx;
	int m_coly;

	int exitrow, exitcol, winrow, wincol;

	int level = 0;
	int m_smellDistance;

public:
	Map(int smellDistance);
	~Map();
	int rows() const;
	int cols() const;

	int exitRow() const;
	int exitCol() const;
	void setExitRow(int coord);
	void setExitCol(int coord);

	int winRow() const;
	int winCol() const;
	void setWinRow(int coord);
	void setWinCol(int coord);

	//getters
	Player* player() const;
	Actor* monster(int index) const;
	GameObject* item(int index) const;

	void placePlayer();
	void placeMonsters(Actor* m);
	void placeObjects(GameObject* m);

	int getSmellDistance() const;

	//setters
	vector<Actor*>& getMonsterVector();
	vector<GameObject*>& getItemVector();


	void nextLevel();
	void endGame();
	void placeStaircase();
	void generateMap();
	void createRoom(const Room& room);
	void connectRooms(const Room& room1, const Room& room2);
	void drawVerticalCorridor(int row, int col1, int col2);
	void drawHorizontalCorridor(int col, int row1, int row2);
	bool noOverlap(const Room& room) const;

	void printMap();
	void updateMap();

	void storeMap();

	char getGrid(int row, int col) const;
	//getter of my grid to use in my algorithm

	bool determineNewPosition(int& r, int& c, char dir) const;

	bool placeDown(int row, int col) const;
	bool goblinPlaceDown(int row, int col) const;

	bool itemPlaceDown(int row, int col) const;


};

#endif
