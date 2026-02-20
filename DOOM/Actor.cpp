#include "Actor.h"
#include "utilities.h"
#include "Map.h"

#include <iostream>
#include <string>

#include <cmath>

using namespace std;

class Map;
class GameObject;
class Game;



Actor::Actor(int row, int col, int hp, int maxhp, Weapon* weapon, int str, int dex, int armor, int sleep, string name, Map* map) { //POINTER TO WEPAON CLASS 
	m_row = row;
	m_col = col;
	m_hp = hp;
	m_maxhp = maxhp;
	m_weapon = weapon;
	m_str = str;
	m_dex = dex;
	m_armor = armor;
	m_sleep = sleep;
	m_name = name;
	m_map = map;
}
Actor::~Actor() {
	//WHAT ELSE AM I SUPPOED TO DELETE

	//	delete m_weapon; //we don't delete m_weapon here or else we would cause the player to delete a dangling pointer after we delete the player's inventory. Therefore each monster will just delete hteir respective weapons. 

}

//getters and setters for row and column coordinates
int Actor::row() const { return m_row; }
int Actor::col() const { return m_col; }
void Actor::setRow(int row) { m_row = row; }
void Actor::setCol(int col) { m_col = col; }

//getters for each actor attribute
int Actor::getHealth() const { return m_hp; }
int Actor::getMaxHealth() const { return m_maxhp; }
Weapon* Actor::getWeapon() const { return m_weapon; }
int Actor::getStrength() const { return m_str; }
int Actor::getDexterity() const { return m_dex; }
int Actor::getArmor() const { return m_armor; }
int Actor::getSleep() const { return m_sleep; }
string Actor::getName() const { return m_name; }
bool Actor::isPassedOut() const { return m_hp <= 0; }

//setter for each attribute
void Actor::setHealth(int health) { m_hp = health; }
void Actor::setMaxHealth(int maxHealth) { m_maxhp = maxHealth; }
void Actor::setWeapon(Weapon* weapon) { m_weapon = weapon; } //this is my setter here
void Actor::setStrength(int strength) { m_str = strength; }
void Actor::setDexterity(int dexterity) { m_dex = dexterity; }
void Actor::setArmor(int armor) { m_armor = armor; }
void Actor::setSleep(int sleep) { m_sleep = sleep; }

//setters for applying a scroll to the player
void Actor::increaseMaxHealth(int increase) { setMaxHealth(getMaxHealth() + increase); }
void Actor::increaseStrength(int increase) { setStrength(getStrength() + increase); }
void Actor::increaseArmor(int increase) { setArmor(getArmor() + increase); }
void Actor::increaseDexterity(int increase) { setDexterity(getDexterity() + increase); }

//Player movement function
void Actor::move(char dir) {
	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}

	if (trueWithProbability(0.1)) {
		if (getHealth() < getMaxHealth()) {
			setHealth(getHealth() + 1);
		}
	}

	vector<Actor*>& monsters = m_map->getMonsterVector(); //return m_monsters; //want a reference to the vector of monsters

	int r = m_row;
	int c = m_col;
	if (m_map->determineNewPosition(r, c, dir)) {
		if (m_map->placeDown(r, c)) {
			setRow(r);
			setCol(c);
		}
		else {
			for (auto index = monsters.begin(); index != monsters.end();) {
				Actor* monster = *index;

				if (r == monster->row() && c == monster->col()) {
					attack(this, monster);

					if (monster->isPassedOut()) {

						decideWhatToDrop(monster);

						Actor* tempPointer = nullptr;
						tempPointer = monster;
						delete tempPointer;

						index = monsters.erase(index);
					}
					else { index++; }
					break;
				}

				index++; //need this in case i dont hit the if statement
			}
		}
	}
}



//trivial name explanation
void Actor::decideWhatToDrop(Actor* monster) {
	vector<GameObject*>& items = m_map->getItemVector();
	GameObject* item = nullptr;

	if (monster->row() == m_map->exitRow() && monster->col() == m_map->exitCol()) {
		return;
	}

	switch (monster->description()) {

	case 'B':
		if (trueWithProbability(0.1)) {
			if (m_map->itemPlaceDown(monster->row(), monster->col())) {
				//drop magic axe
				item = new Magicaxe(monster->row(), monster->col(), m_map);
				items.push_back(item); //over m_map->getItemVector().push_back(item); //because this pushes it into the actual getItemVector due to pass by reference.
				m_map->placeObjects(item);
				break;
			}
		}

	case 'G':
		if (trueWithProbability(0.33)) { //should be 0.33 
			int decideDrop = randInt(2);
			if (m_map->itemPlaceDown(monster->row(), monster->col())) {
				if (decideDrop == 0) {
					item = new Magicaxe(monster->row(), monster->col(), m_map);
					items.push_back(item); //over m_map->getItemVector().push_back(item);
					m_map->placeObjects(item);
					break;
				}
				else {
					item = new Magicfangsofsleep(monster->row(), monster->col(), m_map);
					items.push_back(item); //over m_map->getItemVector().push_back(item);
					m_map->placeObjects(item);
					break;
				}
			}
		}
		break;

	case 'S':
		if (trueWithProbability(0.33)) { //should be 0.33
			if (m_map->itemPlaceDown(monster->row(), monster->col())) {
				//drop magic fangs of sleep
				item = new Magicfangsofsleep(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
		}
		break;

	case 'D':
		int decideDrop = randInt(5); //test finished
		if (m_map->itemPlaceDown(monster->row(), monster->col())) {
			if (decideDrop == 0) {
				item = new ScrollOfEnhanceDexterity(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
			else if (decideDrop == 1) {
				item = new ScrollOfEnhanceHealth(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
			else if (decideDrop == 2) {
				item = new ScrollOfImproveArmor(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
			else if (decideDrop == 3) {
				item = new ScrollOfRaiseStrength(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
			else if (decideDrop == 4) {
				item = new ScrollOfTeleportation(monster->row(), monster->col(), m_map);
				items.push_back(item);
				m_map->placeObjects(item);
				break;
			}
		}
	}
}
//checks distance to the player
double Actor::distanceTo(int row, int col) const { return abs((row - this->row()) + abs(col - this->col())); } //bc we r in taxicab metric
double Actor::otherDistanceTo(int row, int col) const { return sqrt((this->row() - row) * (this->row() - row) + (this->col() - col) * (this->col() - col)); }
//trivial name explanation
void Actor::attack(Actor* perpetrator, Actor* victim) {
	string result = "";

	Weapon* perpetratorWeapon = perpetrator->getWeapon(); //first we take in two pointers to actors. The perpetrator and the victim

	int damage = randInt(0, perpetrator->getStrength() + (perpetratorWeapon->getDamage() - 1)); //damage is calculated according to formulas provided by smolberg
	int attack = perpetrator->getDexterity() + perpetratorWeapon->getDexterity(); //attack points calculated with smolberg.
	int defence = victim->getDexterity() + victim->getArmor(); //get defence points calculated with smolberg

	if (randInt(1, attack) >= randInt(1, defence)) { //check if the attack should land or not

		victim->setHealth(victim->getHealth() - damage); //deal damage to the point

		result = "and hits.";

		if (perpetratorWeapon->getName() == "magic fangs of sleep") {
			bool putToSleep = (trueWithProbability(0.2)); //should be 0.2
			if (putToSleep) {
				int sleepTimer = max(randInt(2, 6), victim->getSleep());
				victim->setSleep(sleepTimer);
				result = "and hits, putting " + (victim->getName()) + " to sleep."; //does this syntax even work //i gues it does
			}
		}
	}
	else { result = "and misses."; }

	if (victim->getHealth() <= 0) { result = "dealing a final blow."; }

	setOutputString(perpetrator->getName() + " " + perpetratorWeapon->getAction() + " " + victim->getName() + " " + result + '\n');  //oututting string

}

//getter for whatever each actor will output
string Actor::getOutputString() { return outputString; }
//setter for whatever each actor will output
void Actor::setOutputString(string input) { outputString = input; }
//basic move function that I did not make pure virtual or else I could not inherit an abstract class
void Actor::move() { return; }
//Monster's move function with smell distance
void Actor::move(int smell) {

	if (this->description() == 'D') { //checking for dragon regen
		if (trueWithProbability(0.1)) {
			if (getHealth() < getMaxHealth()) {
				setHealth(getHealth() + 1);
			}
		}
	}
	if (getSleep() > 0) { //trivial
		setSleep(getSleep() - 1);
		setOutputString("");
		return;
	}
	Player* player = m_map->player();

	if (otherDistanceTo(player->row(), player->col()) <= smell) {


		if (col() < player->col()) { //column movement before row movement because that was what smolberg had done in his code
			if (m_map->placeDown(row(), col() + 1)) { //if i can move there move there
				setCol(col() + 1);
				return;
			}
			else if (row() == player->row() && col() + 1 == player->col()) {
				attack(this, player); //otherwise if the coords line up, attack the player
				return;
			}
		}

		if (col() > player->col()) {
			if (m_map->placeDown(row(), col() - 1)) { //west dir
				setCol(col() - 1);
				return;
			}
			else if (row() == player->row() && col() - 1 == player->col()) {
				attack(this, player);
				return;
			}
		}

		if (row() < player->row()) {
			if (m_map->placeDown(row() + 1, col())) { //south dir
				setRow(row() + 1);
				return;
			}
			else if (row() + 1 == player->row() && col() == player->col()) {
				attack(this, player);
				return;
			}
		}

		if (row() > player->row()) {
			if (m_map->placeDown(row() - 1, col())) { //north dir
				setRow(row() - 1);
				return;
			}
			else if (row() - 1 == player->row() && col() == player->col()) {
				attack(this, player);
				return;
			}
		}

	}
}

Map* Actor::getMap() {
	return m_map;
}
Weapon* Actor::getWeapon() {
	return m_weapon;
}

void Actor::deleteWeapon() {
	delete m_weapon;
}


//######################
//PLAYER IMPLEMENTATIONS
//######################

Player::Player(int row, int col, Map* map)
	: Actor(row, col, 20, 20, new Weapon(0, 0, 2, 0, "short sword", "slashes short sword at", ')', map), 2, 2, 2, 0, "Player", map) {
	m_inventory.push_back(getWeapon()); //starts off with a short sword in inventory.
}
Player::~Player() {
	for (auto item : m_inventory) {
		delete item;
	}
	m_inventory.clear();
}
char Player::description() { return '@'; }

//changes weapons 
void Player::switchWeapons(char input) {
	int index = input - 'a'; // Convert 'a'-'z' to 0-25

	if (index >= 0 && index < m_inventory.size()) {
		// Get the item from the inventory at the given index
		GameObject* selectedItem = m_inventory[index];

		if (selectedItem->description() == '?') {
			setOutputString("You can't wield " + selectedItem->getName() + '\n');
			return;
		}

		// Check if the selected item is a Weapon
		Weapon* selectedWeapon = dynamic_cast<Weapon*>(selectedItem);
		if (selectedWeapon) {
			setWeapon(selectedWeapon);
			setOutputString("You are wielding " + selectedWeapon->getName() + '\n');
		}

	}
}

//uses scroll after iterating through inventory for index
void Player::useScroll(char input) {
	int index = input - 'a';
	if (index >= 0 && index < m_inventory.size()) {
		GameObject* selectedItem = m_inventory[index];
		if (selectedItem->description() == ')') {
			setOutputString("You can't read a " + selectedItem->getName() + '\n');
		}

		if (selectedItem->description() == '?') {
			selectedItem->use(this); //is this even the correct implementation? 
			if (selectedItem->getName() == "scroll of teleportation") {
				int newRow, newCol;
				do {
				   newRow = randInt(1, 17);
				   newCol = randInt(1, 69);
				} while (!getMap()->placeDown(newRow, newCol));
				setRow(newRow);
				setCol(newCol);
			}
			setOutputString("You read the scroll called " + selectedItem->getName() + '\n' + selectedItem->getAction() + '\n');
			m_inventory.erase(m_inventory.begin() + index);
		}
	}
}

//adds an item if I press Q 
void Player::addItem() { //already dealt with all syntax properly

	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}

	vector<GameObject*>& items = getMap()->getItemVector(); //getMap from m_map

	for (auto index = items.begin(); index != items.end();) { //iterator should not be at the top because if so, after deleting something, it points beyond the vector. 
		GameObject* currentItem = *index;
		if (row() == currentItem->row() && col() == currentItem->col()) {

			if (m_inventory.size() > inventorySize) { //cannot have knapsack being full outside of this because if not, it outputs the statement even if im not standing on an item
				//cout inventory full
				setOutputString("Your knapsack is full; you can't pick that up.");
				return;
			}




			m_inventory.push_back(currentItem);

			if (currentItem->description() == '?') {
				setOutputString("You pick up a scroll called " + currentItem->getName() + '\n');
			}
			else if (currentItem->description() == ')') {
				setOutputString("You pick up " + currentItem->getName() + '\n');
			}
			else if (currentItem->description() == '&') {
				setOutputString("You pick up " + currentItem->getName() + '\n' + "Congratulations, you won!" + '\n' + "Press q to exit game." + '\n');
				setWin(true); //sets endgame to true 
			}
			index = items.erase(index); //deletes the item after picking it up.
		}
		else {
			index++;
		}
	}
}

//getter and setters for endgame
bool Player::getWin() const { return won; }
void Player::setWin(bool input) { won = input; }

//getter for players inventory of gameobjects
vector<GameObject*>& Player::getInventory() { return m_inventory; }

void Player::deleteInventory() {

	vector<GameObject*>& inventory = m_inventory;

	for (auto index = inventory.begin(); index != inventory.end();) { //iterator should not be at the top because if so, after deleting something, it points beyond the vector. 
		GameObject* item = *index;
		if (row() == item->row() && col() == item->col()) {

			GameObject* tempPointer = nullptr;
			tempPointer = item;
			//add item to inventory

			GameObject copyItem = *item; //because we cannot do inventory.push_back(tempPointer), we have to create a temporary gameobject as we cannot pass in a pointer. 
			//therefore we must create a temporary pointer to the address of the index and then get the address of that temporary pointer and push it in. 

			GameObject* pointerCopyItem = &copyItem;
			inventory.push_back(pointerCopyItem);

			//delete item from map
			delete tempPointer; //why did it teleport to ~Map();
			index = inventory.erase(index); //HOW DO U DELETE AN ITEM AFTER PICKING UP

		}
		else {
			index++;
		}
	}

}

//prints the inventory when called. 
void Player::printInventory() {
	clearScreen();
	cout << "Inventory: " << endl;
	for (int letter = 0; letter < m_inventory.size(); letter++) {
		char label = 'a' + letter;
		if (m_inventory[letter]->description() == '?') { //check if it is a scroll first 
			cout << " " << label << ". " << "A scroll called " << m_inventory[letter]->getName() << endl;
		}
		else { //else just cout it regularly for weapons
			cout << " " << label << ". " << m_inventory[letter]->getName() << endl;
		}
	}
}

//#############################
//END OF PLAYER IMPLEMENTATIONS
//#############################



//###############
//START OF GOBLIN
//###############

//Basic Goblin unique attributes
Goblin::Goblin(int row, int col, Map* map)
	: Actor(row, col, randInt(15, 20), 20, new Weapon(0, 0, 2, 0, "short sword", "slashes short sword at", ')', map), 3, 1, 1, 0, "the Goblin", map) {}
Goblin::~Goblin() {
	//Weapon* deleteWeapon = getWeapon();
	//delete deleteWeapon;
	deleteWeapon();
}
char Goblin::description() { return 'G'; }

bool Goblin::atPlayer(int row, int col, Pair dest) { //basic test case to chcek if dsesitnation is hit
	if (row == dest.first && col == dest.second) {
		return true;
	}
	return false;
}

//helper function to trace the path from the source to the goblin
pair<int, int> Goblin::tracePath(vector<vector<coord>>& coordDetails, Actor* player) {
	int row = player->row(); //get players pos
	int col = player->col();
	stack<pair<int, int>> Path; //make a stack of coords called path

	//while the coords do not line up, push the path into the stack
	while (!(coordDetails[row][col].parent_row == row && coordDetails[row][col].parent_col == col)) {
		Path.push(make_pair(row, col));
		int temp_row = coordDetails[row][col].parent_row;
		int temp_col = coordDetails[row][col].parent_col;
		row = temp_row;
		col = temp_col;
	}
	Path.push(make_pair(row, col)); //push path in


	// Return the first step towards the player
	Path.pop(); // Remove the player's position
	if (!Path.empty()) {
		return Path.top(); // Return the next position
	}
	else {
		return make_pair(row, col); // Stay in the same place if no path found
	}
}
 
//here is my goblin recursive function
pair<int, int> Goblin::goblinStarSearch(Actor* player, Map* map, int smelldistance) {
	if (abs(row() - player->row()) + abs(col() - player->col()) > smelldistance) { //if out of range, just return
		return make_pair(row(), col());
	}

	const int ROW = 18;
	const int COL = 70;
	vector<vector<bool>> closedList(ROW, vector<bool>(COL, false)); //make a closed list of false meaning no coord has been explored yet implemented as a 2d array
	vector<vector<coord>> coordDetails(ROW, vector<coord>(COL)); //make a 2d array that holds the detail of each coord
	//hold details of each coord
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			coordDetails[i][j].f = 10000; //set it to some obscure high number to cause recursion to go another way
			coordDetails[i][j].g = 10000;
			coordDetails[i][j].h = 10000;
			coordDetails[i][j].parent_row = -1;
			coordDetails[i][j].parent_col = -1;
		}
	}
	//initialize starting nodes parameters
	i = row(), j = col();
	coordDetails[i][j].f = 0.0;
	coordDetails[i][j].g = 0.0;
	coordDetails[i][j].h = 0.0;
	coordDetails[i][j].parent_row = i;
	coordDetails[i][j].parent_col = j;
	//Create an open list having information as <f, <i, j>> where f = g + h, and i, j are the row and column index of that coord
	set<pair<double, pair<int, int>>> openList;

	//insert the starting coord
	openList.insert(make_pair(0.0, make_pair(i, j)));

	//set intial values
	bool foundDest = false;/*
	int inputRow = i++;
	int inputCol = j++;*/ //no longer needed 

	pair<int, int> nextStep = make_pair(row(), col()); //was row and col 

	while (!openList.empty()) {
		pair<double, pair<int, int>> p = *openList.begin();
		openList.erase(openList.begin()); //remove the coord from the openList

		//add the coord to the closedList meaning explored
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;


		double gNew, hNew, fNew;

		vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} }; //get all 4 directions
		for (auto d : directions) {
			int newRow = i + d.first; //calculate each coord pos according to dir
			int newCol = j + d.second;

			//if we hit the destination, set the parent of the destination coord 
			if (newRow == player->row() && newCol == player->col()) {
				coordDetails[newRow][newCol].parent_row = i;
				coordDetails[newRow][newCol].parent_col = j;
				nextStep = tracePath(coordDetails, player); // set the first step to the next step the goblin will take
				foundDest = true;
				return nextStep;
			}
			//else if the successor on the stack is not explorde yet, continue
			if (map->placeDown(newRow, newCol) && !closedList[newRow][newCol]) {
				gNew = coordDetails[i][j].g + 1.0;
				hNew = abs(newRow - player->row()) + abs(newCol - player->col());
				fNew = gNew + hNew; //set heuristic

				//if it is not on the open list yet, add it If it isn’t on the open list, add it to the open list and make the current square the parent of this square. Record the f, g, and h costs of the square cell.
				// lse if it is on the open list already, check to see if this path to that square is better, using 'f' cost as the measure.
				if (coordDetails[newRow][newCol].f == 10000 || coordDetails[newRow][newCol].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(newRow, newCol)));
					coordDetails[newRow][newCol].f = fNew; //update each heurstic of the struct coord
					coordDetails[newRow][newCol].g = gNew;
					coordDetails[newRow][newCol].h = hNew;
					coordDetails[newRow][newCol].parent_row = i;
					coordDetails[newRow][newCol].parent_col = j;
				}
			}
		}
	}

	return nextStep; //return whatever step we must take now
}


//Goblin's move function
void Goblin::goblinMove(int smell) {

	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}

	Player* player = getMap()->player(); //changed to getter

	if (distanceTo(player->row(), player->col()) > smell) {
		return; //break statement to make sure we dont waste time
	}
	//get nextStep
	pair<int, int> nextStep = goblinStarSearch(getMap()->player(), getMap(), getMap()->getSmellDistance());

	if (nextStep.first != player->row() || nextStep.second != player->col()) { //check if we are not at the position yet
		setRow(nextStep.first);
		setCol(nextStep.second);
		// Move to the first step
	}
	else if (nextStep.first == player->row() && nextStep.second == player->col()) { //attack 
		attack(this, player);
	}

}

void Goblin::move() {
	goblinMove(getMap()->getSmellDistance()); //all I really need tbh
}

//#############
//END OF GOBLIN
//#############



//Basic Bogeyman's unique attributes
Bogeyman::Bogeyman(int row, int col, Map* map)
	: Actor(row, col, randInt(5, 10), 10, new Weapon(0, 0, 2, 0, "short sword", "slashes short sword at", ')', map), randInt(2, 3), randInt(2, 3), 2, 0, "the Bogeyman", map) {}
Bogeyman::~Bogeyman() {
	//Weapon* deleteWeapon = getWeapon();
	//delete deleteWeapon;
	deleteWeapon();
}
char Bogeyman::description() { return 'B'; }
void Bogeyman::move() {
	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}
	Actor::move(5);
} //should be 5

//Basic Snakewomen unique attributes
Snakewomen::Snakewomen(int row, int col, Map* map)
	: Actor(row, col, randInt(3, 6), 6, new Weapon(0, 0, 3, 2, "magic fangs of sleep", "strikes magic fangs at", ')', map), 2, 3, 3, 0, "the Snakewoman", map) {}
Snakewomen::~Snakewomen() {
	//Weapon* deleteWeapon = getWeapon();
	//delete deleteWeapon;
	deleteWeapon();
}
char Snakewomen::description() { return 'S'; }
void Snakewomen::move() {

	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}
	Actor::move(3);
} //should be 3

//Basic Dragon unique attributes
Dragon::Dragon(int row, int col, Map* map)
	: Actor(row, col, randInt(20, 25), 25, new Weapon(0, 0, 4, 2, "long sword", "slashes long sword at", ')', map), 4, 4, 4, 0, "the Dragon", map) {
	setMaxHealth(getHealth()); //to implement regeneration
}
Dragon::~Dragon() {
	//Weapon* deleteWeapon = getWeapon();
	//delete deleteWeapon;
	deleteWeapon();
}
char Dragon::description() { return 'D'; }
void Dragon::move() {

	if (getSleep() > 0) {
		setSleep(getSleep() - 1);
		return;
	}

	Actor::move(1);
} //should be 1