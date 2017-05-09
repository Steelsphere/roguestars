#include "Tile.h"
#include "Random.h"

Tile::Tile() {

}

Tile::Tile(int x, int y, int z, TILE_TYPE type, TCODColor color) : Actor(x, y, z) {
	
	switch (type) {

	case GRASS:
		switch (Random::one_to_four(Random::generator)) {
		case 1:
			_c = '.';
			break;
		case 2:
			_c = ',';
			break;
		case 3:
			_c = '`';
			break;
		case 4:
			_c = '\'';
			break;
		}
		switch (Random::coinflip(Random::generator)) {
		case 1:
			_fcolor = TCODColor::darkGreen;
			break;
		case 2:
			_fcolor = TCODColor::green;
			break;
		}
		_bcolor = TCODColor::black;
		_name = "Grass";
		break;

	case STONE_WALL:
		_c = 219;
		_fcolor = TCODColor::grey;
		_bcolor = TCODColor::grey;

		_name = "Stone";
		_impassable = true;
		_transparent = false;
		break;

	case DIRT:
		switch (Random::one_to_four(Random::generator)) {
		case 1:
			_c = '.';
			break;
		case 2:
			_c = ',';
			break;
		case 3:
			_c = '`';
			break;
		case 4:
			_c = '~';
			break;
		}
		if (color == TCODColor::black) {
			_fcolor = TCODColor::sepia;
			_bcolor = TCODColor::black;
		}
		else {
			_fcolor = color;
		}
		_name = "Dirt";
		break;

	case DIRT_WALL:
		_c = 219;
		_fcolor = TCODColor::sepia;
		_bcolor = TCODColor::sepia;

		_name = "Dirt";
		_impassable = true;
		_transparent = false;
		break;

	case WATER:
		_c = '#';
		_fcolor = TCODColor::blue;
		_bcolor = TCODColor::black;

		_name = "Water";
		_impassable = true;
		break;

	case TREE:
		_c = 24;
		_fcolor = TCODColor::lightGreen;
		_bcolor = TCODColor::black;

		_name = "Tree";
		_impassable = true;
		_transparent = false;
		break;
	
	case SAND:
		switch (Random::one_to_four(Random::generator)) {
		case 1:
			_c = '.';
			break;
		case 2:
			_c = ',';
			break;
		case 3:
			_c = '`';
			break;
		case 4:
			_c = '~';
			break;
		}
		_fcolor = TCODColor::yellow;
		_bcolor = TCODColor::black;

		_name = "Sand";
		break;
	}

}
