#include "GameObjects.h"

GameObject::GameObject(int row, int col, string name, string action, char description, Map* map) : m_row(row), m_col(col), m_name(name), m_action(action), m_map(map), m_description(description){
}

GameObject::~GameObject() {
}

//getters and setters for each attribute of a game object
string GameObject::getName() { return m_name; }
string GameObject::getAction() { return m_action; }

int GameObject::row() const { return m_row; }
int GameObject::col() const { return m_col; }
void GameObject::setRow(int row) { m_row = row; }
void GameObject::setCol(int col) { m_col = col; }
char GameObject::description() const { return m_description; }
void GameObject::setDescription(char description) { m_description = description; }

void GameObject::setName(string name) { m_name = name; }
void GameObject::setAction(string action) { m_action = action; }

//virtual function that I cannot make abstract due to inheritance
void GameObject::use(Player* player) { return; }

//construction of the golden idol
Goldenidol::Goldenidol(int row, int col, Map* map) : GameObject(row, col, "the golden idol", "Congratulations, you won!", '&', map), m_map(map) {
    m_map = map;
}
Goldenidol::~Goldenidol() {
}

//weapon with gameobject constructor but three different variables that are unqiue to weapons
Weapon::Weapon(int row, int col, int damage, int dexterity, string name, string action, char description, Map* map) : GameObject(row, col, name, action, ')', map), m_action(action), m_damage(damage), m_dexterity(dexterity){
    m_action = action;
    m_damage = damage;
    m_dexterity = dexterity;
}


//if magifcfangsofsleep during attack, then you apply it in the player class **applied this to the attack function in general
Weapon::~Weapon() {

}

//getters for the three new variables initialized in the weapon constructor
string Weapon::getAction() const { return m_action; }
int Weapon::getDamage() const { return m_damage; }
int Weapon::getDexterity() const { return m_dexterity; }

//mace implementation
Mace::Mace(int row, int col, Map* map) : Weapon(row, col, 2, 0, "mace", "swings mace at", ')', map) {}
Mace::~Mace() {
}

//shortsword implementation
Shortsword::Shortsword(int row, int col, Map* map) : Weapon(row, col, 2, 0, "short sword", "slashes short sword at", ')', map) {}
Shortsword::~Shortsword() {
}

//longsword implementation
Longsword::Longsword(int row, int col, Map* map) : Weapon(row, col, 4, 2, "long sword", "swings long sword at", ')', map) {}
Longsword::~Longsword() {
}

//magicaxe implementation
Magicaxe::Magicaxe(int row, int col, Map* map) : Weapon(row, col, 5, 5, "magic axe", "chops magic axe at", ')', map) {}
Magicaxe::~Magicaxe() {
}

//magic fangs of sleep implementation
Magicfangsofsleep::Magicfangsofsleep(int row, int col, Map* map) : Weapon(row, col, 3, 2, "magic fangs of sleep", "strikes magic fangs of sleep at", ')', map) {}
Magicfangsofsleep::~Magicfangsofsleep() {
}
bool Magicfangsofsleep::putsToSleep() const { return (trueWithProbability(0.20)); }// 1 in 5 chance 
int Magicfangsofsleep::sleepTime() const { return randInt(2, 6); }// Random integer from 2 to 6//add already asleep condition into here if needed. **added this into the attack function

//Scroll constructor which does not need any other variables introduced
Scroll::Scroll(int row, int col, string name, string action, char description, Map* map) : GameObject(row, col, name, action, '?', map) { }

Scroll::~Scroll() {
}

//Initialization of scroll of tp
ScrollOfTeleportation::ScrollOfTeleportation(int row, int col, Map* map) : Scroll(row, col, "scroll of teleportation", "You feel your body wrenched in space and time.", '?', map) {}

//if used, apply the same function of PlaceDown to the player
void ScrollOfTeleportation::use(Player* player)       {  //implementation was moved to player instead

    //int newRow, newCol;
    //do {
    //    newRow = randInt(1, 17);
    //    newCol = randInt(1, 69);
    //} while (!m_map->placeDown(newRow, newCol));
    //player->setRow(newRow);
    //player->setCol(newCol);
}

//initialization of scroll of imrpove armor
ScrollOfImproveArmor::ScrollOfImproveArmor(int row, int col, Map* map) : Scroll(row, col, "scroll of improve armor", "Your armor glows blue.", '?', map) {}

//if used, call players increaseArmor function
void ScrollOfImproveArmor::use(Player* player) {
    player->increaseArmor(randInt(1, 3));
}

//initialization of scroll of raise strength
ScrollOfRaiseStrength::ScrollOfRaiseStrength(int row, int col, Map* map) : Scroll(row, col, "scroll of raise strength", "Your muscles bulge.", '?', map) {}

//if used, call players increase strength function
void ScrollOfRaiseStrength::use(Player* player) {
    player->increaseStrength(randInt(1, 3));
}

//initialization of scroll of enhance health
ScrollOfEnhanceHealth::ScrollOfEnhanceHealth(int row, int col, Map* map) : Scroll(row, col, "scroll of enhance health", "You feel your heart beating stronger.", '?', map) {}

//if used, call players increase max hp function
void ScrollOfEnhanceHealth::use(Player* player) {
    player->increaseMaxHealth(randInt(3, 8));
}


//initialization of scroll of enhance dexterity
ScrollOfEnhanceDexterity::ScrollOfEnhanceDexterity(int row, int col, Map* map) : Scroll(row, col, "scroll of enhance dexterity", "You feel like less of a klutz.", '?', map) {}

void ScrollOfEnhanceDexterity::use(Player* player) {
    player->increaseDexterity(1);
}


