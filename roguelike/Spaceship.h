#pragma once

#include "Actor.h"
#include "Economy.h"

class StarSector;
class Faction;

class Spaceship : public Actor {
public:
	Spaceship(char c, StarSector* s, Faction* f);
	~Spaceship();

	void update();
	void path_to_location(int x, int y);
	void follow_path();

	Economy::Goods cost;
	std::vector<Actor*> path;
};

class Freighter : public Spaceship {
public:
	Freighter(StarSector* s, Faction* f);
};

