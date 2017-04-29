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
	};

	Tile(int x, int y, int z, TILE_TYPE type, TCODColor color = TCODColor::black);
};