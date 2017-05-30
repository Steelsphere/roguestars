#pragma once

#include <libtcod\libtcod.hpp>

#include "Actor.h"


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
		SNOWY_TREE
	};

	Tile();
	
	Tile(int x, int y, int z, TILE_TYPE type, TCODColor color = TCODColor::black);
};

class StarSector : public Actor {
public:
	StarSector(int x, int y, int z);

	virtual void on_pg_down() override;
};

class SolarSystem : public Actor {
public:
	enum SOLAR_TYPE {
		MAIN_SEQUENCE,
		DWARF,
		GIANT,
	};
	
	SolarSystem(int x, int y, int z);

	virtual void on_pg_down() override;
};

class Planet : public Actor {
public:
	enum PLANET_TYPE {
		TERRA,
	};
	Planet(int x, int y, int z, PLANET_TYPE type);

	virtual void on_pg_down() override;
};