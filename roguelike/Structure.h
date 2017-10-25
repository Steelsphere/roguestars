#pragma once
#define TCOD_SDL2
#include "Tile.h"

class Structure {
public:
	enum S_TYPE {
		NONE,
		CABIN,
		TINY_SPACESHIP,
	};
	
	Structure(int x, int y, S_TYPE type);
	~Structure();
};

