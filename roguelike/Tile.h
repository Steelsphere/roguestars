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
	};

	Tile();
	
	Tile(int x, int y, int z, TILE_TYPE type, TCODColor color = TCODColor::black);
};

class Planet : public Actor {
public:
	enum PLANET_TYPE {
		TERRA,
	};
	Planet(int x, int y, int z, PLANET_TYPE type);

	virtual void on_down() override;
};