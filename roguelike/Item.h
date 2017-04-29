#pragma once
#include "Actor.h"

class Item : public Actor
{
public:
	enum ITEM_TYPE {
		FLOWER,
		BUSH,
	};
	
	Item(int x, int y, int z, Item::ITEM_TYPE type);
	~Item();
};

