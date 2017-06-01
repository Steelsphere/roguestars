#include "Tile.h"
#include "Random.h"
#include "GameEvent.h"
#include "Level.h"

#include <iostream>

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
			_fcolor = TCODColor::darkestGreen;
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
		_fcolor = TCODColor::lightYellow;
		_bcolor = TCODColor::black;

		_name = "Sand";
		break;
	
	case SANDSTONE:
		_c = 219;
		_fcolor = TCODColor::desaturatedYellow;
		_bcolor = TCODColor::desaturatedYellow;

		_impassable = true;
		_transparent = false;

		_name = "Sandstone";
		break;
	
	case SPACE:
		_c = 0;
		_fcolor = TCODColor::black;
		_bcolor = TCODColor::black;

		_name = "Space";
		break;
	
	case STAR:
		_c = '*';
		_fcolor = TCODColor::yellow;
		_bcolor = TCODColor::black;

		_impassable = true;

		_name = "Star";
		break;

	case PLANET:
		_c = 15;
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;

		_name = "Planet";
		break;

	case DISTANT_STAR:
		_c = '.';
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;

		_name = "Distant Star";
		break;

	case STAR_DUST:
		_c = 0;
		_fcolor = TCODColor::black;
		_bcolor = TCODColor(Random::one_to_sixty_four(Random::generator), 0, Random::one_to_sixty_four(Random::generator));

		_name = "Star Dust";
		break;
	
	case SNOW:
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
		
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;
		_name = "Snow";
		break;

	case ICE:
		_c = '#';
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;

		_name = "Ice";
		break;

	case SNOWY_TREE:
		_c = 24;
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;

		_name = "Tree";
		_impassable = true;
		_transparent = false;
		break;
	}


}

StarSector::StarSector(int x, int y, int z) : Actor(x, y, z) {
	_c = '*';
	_fcolor = TCODColor::white;
	_bcolor = TCODColor::black;

	_name = "Star Sector";
}

void StarSector::on_pg_down() {
	GameEvent::set_event(GameEvent::NEW_STAR_SECTOR);
}

SolarSystem::SolarSystem(int x, int y, int z) : Actor(x, y, z) {
	_c = '*';
	_fcolor = TCODColor::white;
	_bcolor = TCODColor::black;

	_name = "Star";
}

void SolarSystem::on_pg_down() {
	GameEvent::set_event(GameEvent::NEW_SOLAR_SYSTEM);
}

Planet::Planet(int x, int y, int z, PLANET_TYPE type) : Actor(x, y, z) {
	_c = 15;

	switch (type) {
	case TERRA:
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;

		_name = "Planet";
	}

}

void Planet::on_pg_down() {
	GameEvent::set_event(GameEvent::NEW_WORLD);
}

Biome::Biome(int x, int y, int z, int type) : Actor(x, y, z) {
	switch (type) {
	case 0:
		_c = '#';
		_fcolor = TCODColor::blue;
		_bcolor = TCODColor::black;
		_name = "Ocean";
		_impassable = true;
		break;
	case Level::GRASSLAND:
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
			_fcolor = TCODColor::darkestGreen;
			break;
		}
		_bcolor = TCODColor::black;
		_name = "Grasslands";
		break;
	case Level::FOREST:
		_c = 24;
		_fcolor = TCODColor::lightGreen;
		_bcolor = TCODColor::black;

		_name = "Forest";
		break;
	case Level::HILLS:
		_c = '^';
		_fcolor = TCODColor::sepia;
		_bcolor = TCODColor::black;

		_name = "Hills";
		break;
	case Level::DESERT:
		_c = '~';
		_fcolor = TCODColor::yellow;
		_bcolor = TCODColor::black;
		_name = "Desert";
		break;
	case Level::SNOWY_TAIGA:
		_c = 24;
		_fcolor = TCODColor::white;
		_bcolor = TCODColor::black;
		_name = "Taiga";
		break;
	}
}

void Biome::on_pg_down() {

}