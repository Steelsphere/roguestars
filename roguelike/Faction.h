#pragma once
#define TCOD_SDL2

#include "Tile.h"
#include "Hero.h"

#include <vector>

class Faction {
	friend class Hero;

public:
	std::vector<std::string> prefixes = {
		"Republic of ",
		"Federation of ",
		"Confederation of ",
		"Union of ",
		"Democratic Republic of ",
		"People's Republic of ",
		"Democratic People's Republic of ",
		"Nation of ",
		"State of ",
		"Democracy of ",
		"Empire of ",
		"Collective of ",
		"United Republic of ",
		"United States of ",
		"United Empire of ",
		"Glorious Republic of ",
		"Free State of ",
		"Military Junta of ",
		"Army of ",
		"Glorious Empire of ",
		"Glorious People's Republic of ",
		"Freedom Fighters of ",
		"Government of ",
		"Armed Forces of ",
		"Dictatorship of ",
		"Rogue Nation of ",
		"Rogue State of "
	};
	std::vector<std::string> suffixes = {
		" Republic",
		" Federation",
		" Confederation",
		" Union",
		" Nation",
		" State",
		" Government",
		" Empire",
		" Collective",
		" Free State",
		" Army",
		" Armed Forces",
		" Military Junta",
		" Dictatorship",
		" Police State",
		" Corporatocracy",
		" Freedom Fighters",
		" Resistance",
	};
	
	Faction();
	Faction(int startx, int starty);
	
	~Faction();

	static std::vector<Faction*> get_factions() { return _factions; }

	static bool any_own_tile(const Actor* t);

	static Faction* who_owns_tile(const Actor* t);

	static void save_faction_map(const std::string& path, int size);

	static std::vector<Actor*>* get_actors() { return &_galaxy_actors; }

	static void set_actors_cpy(std::vector<Actor*>* v);

	static void reinit_factions();

	TCODColor get_color() { return _color; }

	std::vector<Actor*> get_owned_tiles() { return _owned_tiles; }

	void simulate();

	bool self_own_tile(const Actor* t);

	bool other_own_tile(const Actor* t);

	void remove_tile(const Actor* t);

	Actor* get_capital() { return _capital_tile; }

	void set_speed(bool fast) { _fast = fast; }

	std::vector<Hero*> get_heroes() { return _heroes; }

	std::string get_name() { return _name; }

private:
	static std::vector<Faction*> _factions;
	
	// This var is for when the the galaxy needs to be loaded
	static std::vector<Actor*> _galaxy_actors;

	std::vector<Actor*> _owned_tiles;
	Actor* _capital_tile;
	TCODColor _color;
	std::string _name;
	std::vector<Hero*> _heroes;

	bool _fast;
};

