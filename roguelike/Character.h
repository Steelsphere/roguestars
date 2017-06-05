#pragma once
#include "Actor.h"

#include <functional>


class Character : public Actor {
public:
	
	Character();
	Character(int x, int y, int z);
	Character(int x, int y, int z, char c, TCODColor color, const std::string& name = "Character");
	~Character();

protected:
	int _health;
	int _speed;
};

class Monster : public Character {
public:
	Monster(int x, int y, int z);
};

