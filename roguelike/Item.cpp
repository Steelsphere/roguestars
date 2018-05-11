#include "Item.h"
#include "Random.h"


Item::Item() {

}

Item::Item(int x, int y, int z, Item::ITEM_TYPE type) : Actor(x, y, z) {
	_transparent = true;

	switch (type) {
	case FLOWER:
		_c = '.';
		switch (Random::coinflip(Random::generator)) {
		case 1:
			_fcolor = TCODColor::red;
			break;
		case 2:
			_fcolor = TCODColor::yellow;
			break;
		}
		_bcolor = TCODColor::black;
		_name = "Flower";
		break;
	
	case BUSH:
		_c = '*';
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;
		_name = "Bush";
		break;
	
	case CACTUS:
		_c = '*';
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;
		_impassable = true;
		_name = "Cactus";
		break;

	case DEAD_BUSH:
		_c = '*';
		_fcolor = TCODColor::desaturatedYellow;
		_bcolor = TCODColor::black;
		_name = "Bush";
		break;

	case SNOW_BUSH:
		_c = '*';
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;
		_name = "Bush";
		break;

	case DIGITAL_WATCH:
		_c = 'o';
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;
		_name = "Digital Watch";
		break;
	}
}


Item::~Item() {}

void Item::draw() {
	if (!_is_in_inv) {
		Actor::draw();
	}
}

void Item::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(_is_in_inv);
}

void Item::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	_is_in_inv = zip->getInt();
}
