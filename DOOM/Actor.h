#ifndef ACTOR_H
#define ACTOR_H
#include <iostream>
#include "Map.h"
#include "utilities.h"
#include "GameObjects.h"

#include <vector>
#include <list>
#include <cmath>

#include <stack>
#include <set>

using namespace std;

class Map;
class Weapon;
class GameObject;

class Actor {

public:
	Actor(int row, int col, int hp, int maxhp, Weapon* weapon, int str, int dex, int armor, int sleep, string name, Map* map); //basic constructor? 

	virtual ~Actor(); //destructor
	virtual char description() = 0; //pure virtual function

	//positions helper functions
	int row() const; //getter functions
	int col() const; //getter functions
	void setRow(int row);
	void setCol(int col);

	//status bar	
	int getHealth() const;
	int getMaxHealth() const;
	Weapon* getWeapon() const;
	int getStrength() const;
	int getDexterity() const;
	int getArmor() const;
	int getSleep() const;
	string getName() const;

	//game over condition
	bool isPassedOut() const;

	void setHealth(int health);
	void setMaxHealth(int health);
	void setWeapon(Weapon* weapon);
	void setStrength(int strength);
	void setDexterity(int dexterity);
	void setArmor(int armor);
	void setSleep(int sleep);

	void increaseMaxHealth(int maxhealth);
	void increaseStrength(int strength);
	void increaseArmor(int armor);
	void increaseDexterity(int dexterity);


	void decideWhatToDrop(Actor* monster);

	//attack functions
	void attack(Actor* perpetrator, Actor* victim);
	string getOutputString();
	void setOutputString(string input);

	virtual void move(char dir); //didnt make it virtual  //make it so that when player and dragon move, they have 10% chance of healing 1 hp	
	virtual void move();
	virtual void move(int smell);


	//void moveTowards(int row, int col, int smell);
	double distanceTo(int row, int col) const; //heuristic for goblin
	double otherDistanceTo(int row, int col) const; //this is for the other 3 monsters not goblin

	//void wieldWeapon( with a pointer to whatever weapon we have );

	Map* getMap();
	Weapon* getWeapon();
	void deleteWeapon();


	 
private://protected
	Map* m_map;
	Weapon* m_weapon;
	int m_row, m_col, m_hp, m_maxhp, m_str, m_dex, m_armor, m_sleep;
	string m_name, outputString;
};


class Player : public Actor {
public:
	Player(int row, int col, Map* map);
	~Player();
	virtual char description();

	void addItem();
	void deleteInventory();
	void printInventory();
	vector<GameObject*>& getInventory();
	void switchWeapons(char input);
	void useScroll(char input);
	bool getWin() const;
	void setWin(bool input);

private: //protected
	bool won = false;
	int inventorySize = 25;
	vector<GameObject*> m_inventory; //should have size 25.
};


class Bogeyman : public Actor {
public:
	Bogeyman(int row, int col, Map* map); //1 in 10 chance to drop magic axe
	~Bogeyman();
	virtual char description();
	virtual void move();
private:
};

class Goblin : public Actor {
private:

	typedef pair <int, int> Pair;
	typedef pair <double, pair<int, int> > pPair;

	struct coord {
		// Row and Column index of its parent
		// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
		int parent_row, parent_col;
		// f = g + h
		double f, g, h;
	};


public:
	Goblin(int row, int col, Map* map); //1 in 3 chance to drop magic axe or magic fangs of sleep
	~Goblin();
	virtual char description();
	virtual void move();

	virtual void goblinMove(int smell);

	//some recursive function here
	//int calculateHValue(int row, int col); //this is easily replaced by my already existing distanceTo function which also has a smell parameter

	//need a star search

	bool atPlayer(int row, int col, Pair dest);

	pair<int, int> tracePath(vector<vector<coord>>& coordDetails, Actor* player);

	pair<int, int> goblinStarSearch(Actor* player, Map* map, int smelldistance);





};

class Snakewomen : public Actor {
public:
	Snakewomen(int row, int col, Map* map); //1/3 chance to drop an item
	~Snakewomen();
	virtual char description();
	virtual void move();

private:
};


class Dragon : public Actor {
public:
	Dragon(int row, int col, Map* map); //implement 1 in 10 chance of healing 1 hp //implement dropping a scroll with 100% probability if it is possible to place one down. 
	~Dragon();
	virtual char description();
	virtual void move();
private:
};

#endif
