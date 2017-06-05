#include "Character.h"



Character::Character() {}

Character::Character(int x, int y, int z) : Actor(x, y, z) {
	_impassable = true;
	_transparent = false;
}

Character::Character(int x, int y, int z, char c, TCODColor color, const std::string& name) : Actor(x, y, z, c, color, TCODColor::black, name) {
	_impassable = true;
	_transparent = false;
}

Character::~Character() {}

Monster::Monster(int x, int y, int z) : Character(x, y, z) {
	_c = 'M';
	_fcolor = TCODColor::brass;
	_name = "Monster";
}