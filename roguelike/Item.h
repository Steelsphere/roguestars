#pragma once
#include "Actor.h"

class Item : public Actor
{
public:
	enum ITEM_TYPE {
		FLOWER,
		BUSH,
		CACTUS,
		DEAD_BUSH,
		SNOW_BUSH,
	};
	
	Item();
	Item(int x, int y, int z, Item::ITEM_TYPE type);
	~Item();
};

