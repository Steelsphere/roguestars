#pragma once
#include "Tile.h"

class Structure {
public:
	enum S_TYPE {
		CABIN,
		TINY_SPACESHIP,
	};
	
	Structure(int x, int y, S_TYPE type);
	~Structure();
};

