#pragma once
#define TCOD_SDL2

#include <libtcod.hpp>

#include "Actor.h"
#include "Economy.h"

enum LEVEL_TYPE;
class Faction;

class Tile : public Actor {
public:
	enum TILE_TYPE {
		GRASS,
		STONE_WALL,
		DIRT,
		DIRT_WALL,
		WATER,
		TREE,
		SAND,
		SANDSTONE,
		SPACE,
		STAR,
		PLANET,
		DISTANT_STAR,
		STAR_DUST,
		SNOW,
		ICE,
		SNOWY_TREE,
		WOOD,
		GLASS,
		STEEL_WALL,
		STEEL_FLOOR,
	};

	Tile();
	
	Tile(int x, int y, int z, TILE_TYPE type, TCODColor color = TCODColor::black);
};

class TravelPoint : public Actor {
public:
	TravelPoint(int x, int y, int z);
	TravelPoint() : Actor() {}
	
	int id;
	Economy economy;
	std::string alias = TCODNamegen::generate("object");
	bool colonized = false;

	void colonize(Faction* f);
};

class StarSector : public TravelPoint {
public:
	StarSector() {}
	StarSector(int x, int y, int z);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class SolarSystem : public TravelPoint {
public:
	enum SOLAR_TYPE {
		MAIN_SEQUENCE,
		DWARF,
		GIANT,
	};
	
	SolarSystem() {}
	SolarSystem(int x, int y, int z);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class Planet : public TravelPoint {
public:
	enum PLANET_TYPE {
		TERRA,
	};
	
	Planet() {}
	Planet(int x, int y, int z, PLANET_TYPE type);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class Biome : public TravelPoint {
public:
	Biome() {}
	Biome(int x, int y, int z, int type);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class Door : public Actor {
public:
	enum DOOR_TYPE {
		WOOD,
		STEEL,
	};
	
	Door();
	Door(int x, int y, int z, int type);

	virtual void on_collide() override;
	virtual void on_keypress_c() override;
private:
	bool _isopen = false;
};