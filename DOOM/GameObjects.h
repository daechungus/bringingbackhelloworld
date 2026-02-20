#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H


#include "utilities.h"
#include "Map.h"

using namespace std;
#include <iostream>
#include <string>


class Player;
class Map;

class GameObject {
public:
    GameObject(int row, int col, string name, string action, char description, Map* map);
    virtual ~GameObject();

    string getName();
    string getAction();

    void setName(string name);
    void setAction(string name);

    int row() const; //getter functions
    int col() const; //getter functions
    void setRow(int row);
    void setCol(int col);

    char description() const;
    void setDescription(char description);

    virtual void use(Player* player);

private:
    Map* m_map;
    string m_name, m_action;
    int m_row, m_col;
    char m_description;

//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class Goldenidol : public GameObject {
public:
    Goldenidol(int row, int col, Map* map);
    virtual ~Goldenidol();

private: //protected
    Map* m_map;
    string m_name, m_action;
};

class Weapon :public GameObject {
public:
    Weapon(int row, int col, int damage, int dexterity, string name, string action, char description, Map* map);
    virtual ~Weapon();

    string getAction() const;
    int getDamage() const;
    int getDexterity() const;
    //already inherited description from gameobject class 

private:
    int m_damage, m_dexterity;
    string m_action;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class Mace : public Weapon {
public:
    Mace(int row, int col, Map* map);
    virtual ~Mace();
//private:
//    int m_damage, m_dexterity;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class Shortsword : public Weapon {
public:
    Shortsword(int row, int col, Map* map);
    virtual ~Shortsword();
//private:
//    int m_damage, m_dexterity;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class Longsword : public Weapon {
public:
    Longsword(int row, int col, Map* map);
    virtual ~Longsword();
//private:
//    int m_damage, m_dexterity;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;

};

class Magicaxe : public Weapon {
public:
    Magicaxe(int row, int col, Map* map);
    virtual ~Magicaxe();
//private:
//    int m_damage, m_dexterity;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class Magicfangsofsleep : public Weapon {
public:
    Magicfangsofsleep(int row, int col, Map* map);
    virtual ~Magicfangsofsleep();
    bool putsToSleep() const;
    int sleepTime() const;
//private:
//    int m_damage, m_dexterity;
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};


class Scroll : public GameObject {
public:
    Scroll(int row, int col, string name, string action, char description, Map* map);
    ~Scroll();
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class ScrollOfTeleportation : public Scroll {
public:
    ScrollOfTeleportation(int row, int col, Map* map);
    virtual void use(Player* player);
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};


class ScrollOfImproveArmor : public Scroll {
public:
    ScrollOfImproveArmor(int row, int col, Map* map);
    virtual void use(Player* player);
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class ScrollOfRaiseStrength : public Scroll {
public:
    ScrollOfRaiseStrength(int row, int col, Map* map);
    virtual void use(Player* player);
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class ScrollOfEnhanceHealth : public Scroll {
public:
    ScrollOfEnhanceHealth(int row, int col, Map* map);
    virtual void use(Player* player);
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};

class ScrollOfEnhanceDexterity : public Scroll {
public:
    ScrollOfEnhanceDexterity(int row, int col, Map* map);
    virtual void use(Player* player);
//
//private: //protected
//    Map* m_map;
//    string m_name, m_action;
//    int m_row, m_col;
//    char m_description;
};



#endif
