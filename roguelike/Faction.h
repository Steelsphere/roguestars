#pragma once

#include "Tile.h"

#include <vector>

class Faction {
public:
	Faction();
	Faction(int startx, int starty);
	
	~Faction();

	static std::vector<Faction*> get_factions() { return _factions; }

	static bool does_faction_own_tile(int x, int y);

	TCODColor get_color() { return _color; }

	std::vector<Actor*> get_owned_tiles() { return _owned_tiles; }

	void simulate();

	bool self_own_tile(int x, int y);

private:
	static std::vector<Faction*> _factions;

	std::vector<Actor*> _owned_tiles;
	Actor* _capital_tile;
	TCODColor _color;
	std::string _name;
};

