#pragma once

#define FREIGHTER_MAX_CARGO 1000

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
	Faction* faction;
};

class Freighter : public Spaceship {
public:
	enum FREIGHTER_ACTION {
		NONE,
		PICKUP,
		UNLOAD,
		IDLE
	};

	Freighter(StarSector* s, Faction* f);

	virtual void update() override;

	void route(StarSector* start, StarSector* end, Economy::Goods g);

	Economy::Goods cargo;
	FREIGHTER_ACTION action = NONE;

	StarSector* load_dest = nullptr;
	StarSector* unload_dest = nullptr;

private:
	Economy::Goods _willpickup;
};

class Scout : public Spaceship {
public:
	enum SCOUT_ACTION {
		NONE,
		SCOUT_BEGIN,
		SCOUTING
	};

	Scout(StarSector* s, Faction* f);

	virtual void update() override;

	SCOUT_ACTION action = NONE;
};

class Warship : public Spaceship {
public:
	enum MILSHIP_ACTION {
		NONE,
		PATROL_BEGIN,
		PATROLLING,
		ATTACK_BEGIN,
		ATTACKING,
		DEFEND_BEGIN,
		DEFENDING
	};

	Warship(StarSector* s, Faction* f);

	virtual void update() override;

	MILSHIP_ACTION action = NONE;

private:
	std::vector<StarSector*> _patrol_points;
};
