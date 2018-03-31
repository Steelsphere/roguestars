#pragma once
#define TCOD_SDL2
#include "Actor.h"

class Item : public Actor {
public:
	enum ITEM_TYPE {
		FLOWER,
		BUSH,
		CACTUS,
		DEAD_BUSH,
		SNOW_BUSH,
		DIGITAL_WATCH,
	};

	Item();
	Item(int x, int y, int z, Item::ITEM_TYPE type);
	~Item();

	virtual void draw() override;

	bool in_inventory() { return _is_in_inv; }
	void set_in_inventory(bool s) { _is_in_inv = s; }

protected:
	bool _is_in_inv = false;
};
