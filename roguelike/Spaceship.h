#pragma once

#include "Actor.h"
#include "Economy.h"

class StarSector;
class Faction;

class Spaceship : public Actor {
public:
	Spaceship(char c, StarSector* s, Faction* f);
	~Spaceship();

	virtual void update();
	void path_to_location(int x, int y);
	void follow_path();

	Economy::Goods cost;
	std::vector<Actor*> path;
};

class Freighter : public Spaceship {
public:
	enum FREIGHTER_ACTION {
		NONE,
		PICKUP,
		UNLOAD
	};
	
	Freighter(StarSector* s, Faction* f);

	virtual void update() override;

	void pickup_cargo(StarSector* s, Economy::Goods g);
	void unload_cargo(StarSector* s);

	Economy::Goods cargo;
	FREIGHTER_ACTION action = NONE;

private:
	StarSector* _dest = nullptr;
	Economy::Goods _willpickup;
};

