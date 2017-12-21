#pragma once

#include "Actor.h"

class StarSector;
class Faction;
class Economy::Goods;

class Spaceship : public Actor {
public:
	Spaceship(int x, int y, char c, StarSector* s, Faction* f);
	~Spaceship();

	Economy::Goods cost;
};

