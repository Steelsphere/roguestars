#pragma once
#include "Tile.h"

class Structure {
public:
	enum S_TYPE {
		CABIN,
	};
	
	Structure(int x, int y, int w, int h, S_TYPE type, Tile::TILE_TYPE wall, Tile::TILE_TYPE floor);
	~Structure();
};

