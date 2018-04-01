#include "Tile.h"
#include "Random.h"
#include "GameEvent.h"
#include "Level.h"
#include "GameObjects.h"
#include "Faction.h"

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
		default:
			_c = '.';
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
		_c = 1;
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
	
	/*
	case SPACE:
		_c = 0;
		_fcolor = TCODColor::black;
		_bcolor = TCODColor::black;

		_name = "Space";
		break;
	*/

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
	
	case WOOD:
		_c = '#';
		_fcolor = TCODColor::lightSepia;
		_bcolor = TCODColor::black;

		_name = "Wood Wall";
		_impassable = true;
		_transparent = false;
		break;
	
	case GLASS:
		_c = '#';
		_fcolor = TCODColor::lightBlue;
		_bcolor = TCODColor::black;

		_name = "Glass";
		_impassable = true;
		_transparent = true;
		break;

	case STEEL_WALL:
		_c = '#';
		_fcolor = TCODColor::lighterGrey;
		_bcolor = TCODColor::black;

		_name = "Steel Wall";
		_impassable = true;
		_transparent = false;
		break;

	case STEEL_FLOOR:
		_c = '.';
		_fcolor = TCODColor::grey;
		_bcolor = TCODColor::black;

		_name = "Steel Floor";
		break;
	}


}

TravelPoint::TravelPoint(int x, int y, int z) : Actor(x, y, z) {}

void TravelPoint::colonize(Faction* f) {
	economy.supply.food = Random::randc(0, 1000);
	economy.supply.air = Random::randc(0, 1000);
	economy.supply.minerals = Random::randc(0, 1000);
	economy.supply.water = Random::randc(0, 1000);
	economy.supply.workers = Random::randc(10, 100);

	colonized = true;
	faction = f;
}

void TravelPoint::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(id);
}

void TravelPoint::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
}

StarSector::StarSector(int x, int y, int z) : TravelPoint(x, y, z) {
	_c = '*';
	_fcolor = TCODColor::white;
	_bcolor = TCODColor::black;
	id = Random::random(Random::generator);

	_name = "Star Sector";
}

void StarSector::on_pg_down() {
	std::cout << "ID: " << id << std::endl;
	if (GameObjects::file_in_filesystem(GameObjects::savegame_path, std::to_string(id))) {
		GameObjects::level_id_to_load = id;
		GameEvent::set_event(GameEvent::LOAD_LEVEL);
		return;
	}
	std::cout << "Generating new level\n";
	GameObjects::new_level_id = id;
	GameEvent::set_event(GameEvent::NEW_STAR_SECTOR);
}

void StarSector::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(id);
}

void StarSector::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	id = zip->getInt();
}

SolarSystem::SolarSystem(int x, int y, int z) : TravelPoint(x, y, z) {
	_c = '*';
	_fcolor = TCODColor::white;
	_bcolor = TCODColor::black;
	id = Random::random(Random::generator);

	_name = "Star";
}

void SolarSystem::on_pg_down() {
	std::cout << "ID: " << id << std::endl;
	if (GameObjects::file_in_filesystem(GameObjects::savegame_path, std::to_string(id))) {
		GameObjects::level_id_to_load = id;
		GameEvent::set_event(GameEvent::LOAD_LEVEL);
		return;
	}
	std::cout << "Generating new level\n";
	GameObjects::new_level_id = id;
	GameEvent::set_event(GameEvent::NEW_SOLAR_SYSTEM);
}

void SolarSystem::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(id);
}

void SolarSystem::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	id = zip->getInt();
}

Planet::Planet(int x, int y, int z, PLANET_TYPE type) : TravelPoint(x, y, z) {
	_c = 15;
	id = Random::random(Random::generator);

	switch (type) {
	case TERRA:
		_fcolor = TCODColor::green;
		_bcolor = TCODColor::black;

		_name = "Planet";
	}

}

void Planet::on_pg_down() {
	std::cout << "ID: " << id << std::endl;
	if (GameObjects::file_in_filesystem(GameObjects::savegame_path, std::to_string(id))) {
		GameObjects::level_id_to_load = id;
		GameEvent::set_event(GameEvent::LOAD_LEVEL);
		return;
	}
	std::cout << "Generating new level\n";
	GameObjects::new_level_id = id;
	GameEvent::set_event(GameEvent::NEW_WORLD);
}

void Planet::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(id);
}

void Planet::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	id = zip->getInt();
}


Biome::Biome(int x, int y, int z, int type) : TravelPoint(x, y, z) {
	id = Random::random(Random::generator);
	
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

void Biome::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(id);
}

void Biome::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	id = zip->getInt();
}

void Biome::on_pg_down() {
	std::cout << "ID: " << id << std::endl;
	if (GameObjects::file_in_filesystem(GameObjects::savegame_path, std::to_string(id))) {
		GameObjects::level_id_to_load = id;
		GameEvent::set_event(GameEvent::LOAD_LEVEL);
		return;
	}
	std::cout << "Generating new level\n";
	GameObjects::new_level_id = id;
	GameEvent::set_event(GameEvent::ENTER_WORLD_TILE);
}

Door::Door() {}

Door::Door(int x, int y, int z, int type) : Actor(x, y, z) {
	switch (type) {
	case WOOD:
		_c = '\\';
		_fcolor = TCODColor::sepia;
		_bcolor = TCODColor::black;
		_name = "Wood Door";
		_transparent = false;
		_impassable = true;
		break;
	case STEEL:
		_c = '\\';
		_fcolor = TCODColor::grey;
		_bcolor = TCODColor::black;
		_name = "Steel Door";
		_transparent = false;
		_impassable = true;
		break;
	}
}

void Door::on_collide() {
	if (!_isopen) {
		_c = '.';
		_transparent = true;
		_impassable = false;
		_isopen = true;
		GameObjects::update = true;
	}
}

void Door::on_keypress_c() {
	if (_isopen) {
		_c = '\\';
		_transparent = false;
		_impassable = true;
		_isopen = false;
		GameObjects::update = true;
	}
	else {
		_c = '.';
		_transparent = true;
		_impassable = false;
		_isopen = true;
		GameObjects::update = true;
	}
}

void Door::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(_isopen);
}

void Door::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	_isopen = zip->getInt();
}

Space::Space(int x, int y) : Actor(x, y, 0) {
	_c = 0;
	_fcolor = TCODColor::black;
	_bcolor = TCODColor::black;

	_name = "Space";
}

void Space::serialize(TCODZip* zip) {
	Actor::serialize(zip);
}

void Space::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
}