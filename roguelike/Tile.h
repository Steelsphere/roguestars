#pragma once

#include <libtcod\libtcod.hpp>

#include "Actor.h"

enum LEVEL_TYPE;

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

class StarSector : public Actor {
public:
	int id;
	
	StarSector() {}
	StarSector(int x, int y, int z);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class SolarSystem : public Actor {
public:
	int id;
	
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

class Planet : public Actor {
public:
	int id;
	
	enum PLANET_TYPE {
		TERRA,
	};
	
	Planet() {}
	Planet(int x, int y, int z, PLANET_TYPE type);

	virtual void on_pg_down() override;

	virtual void serialize(TCODZip* zip) override;
	virtual void deserialize(TCODZip* zip) override;
};

class Biome : public Actor {
public:
	int id;
	
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