#pragma once

#include "Tile.h"

#include <vector>

class Faction {
public:
	Faction();
	Faction(int startx, int starty);
	
	~Faction();

	static std::vector<Faction*> get_factions() { return _factions; }

	static bool any_own_tile(const Actor* t);

	static Faction* who_owns_tile(const Actor* t);

	static void save_faction_map(const std::string& path, int size);

	TCODColor get_color() { return _color; }

	std::vector<Actor*> get_owned_tiles() { return _owned_tiles; }

	void simulate();

	bool self_own_tile(const Actor* t);

	bool other_own_tile(const Actor* t);

	void remove_tile(const Actor* t);

	Actor* get_capital() { return _capital_tile; }

private:
	static std::vector<Faction*> _factions;

	std::vector<Actor*> _owned_tiles;
	Actor* _capital_tile;
	TCODColor _color;
	std::string _name;
};

