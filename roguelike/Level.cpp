#include "Level.h"
#include "Tile.h"
#include "Item.h"
#include "GameObjects.h"
#include "Structure.h"
#include "Character.h"

#include <fstream>
#include <cstdlib>
#include <Ctime>
#include <string>

TCODMap* Level::_fovmap = nullptr;

Level::Level() {}

Level::~Level() {
	for (int i = 0; i < _actors.size() - 1; i++) {
		delete _actors[i];
	}
	delete _fovmap;
	(*Character::get_chbuff()).clear();
	Actor::set_buffer(nullptr);
	Actor::set_map(nullptr);
}

void Level::generate_level(int size, LEVEL_TYPE type) {
	_width = size / 2;
	_height = size / 2;

	_noise.SetSeed(time(NULL));
	_noise.SetNoiseType(FastNoise::PerlinFractal);
	
	Actor::set_buffer(&_actors);
	Actor::set_map(&_map);

	_map.resize(_height);

	for (int i = 0; i < _map.size(); i++) {
		_map[i].resize(_width);
	}
	
	switch (type) {
	
	case GALAXY:
		generate_space();
		generate_space_obj(Random::one_to_sixteen, GALAXY);
		break;

	case STAR_SECTOR:
		generate_space();
		generate_space_obj(Random::one_to_thirty_two, STAR_SECTOR);
		break;

	case SOLAR_SYSTEM:
		generate_space();
		generate_space_obj(Random::one_to_thirty_two, SOLAR_SYSTEM);
		break;
	
	case SPACE:
		generate_space();
		generate_space_obj(Random::one_to_thirty_two, SPACE);
		break;
	
	case GRASSLAND:
		generate_terrain(0.01f, -0.25f, 0.25f, 0.01f,
			Tile::WATER,
			Tile::GRASS,
			Tile::DIRT_WALL,
			Tile::DIRT);
		generate_trees(Random::one_to_one_twenty_eight);
		generate_flora(Random::one_to_one_twenty_eight);
		break;
	
	case HILLS:
		generate_terrain(0.01f, 0.0f, 0.25f, 0.01f,
			Tile::WATER,
			Tile::GRASS,
			Tile::DIRT_WALL,
			Tile::DIRT);
		generate_trees(Random::one_to_sixteen);
		generate_flora(Random::one_to_sixty_four);
		break;
	
	case FOREST:
		generate_terrain(0.01f, -0.25f, 0.25f, 0.01f,
			Tile::WATER,
			Tile::GRASS,
			Tile::DIRT_WALL,
			Tile::DIRT);
		generate_structures(Random::randc(0, 250));
		generate_trees(Random::one_to_eight);
		generate_flora(Random::one_to_two_fifty_six);
		break;
	
	case DESERT:
		generate_terrain(0.01f, -0.25f, 0.25f, 0.01f, 
			Tile::SAND, 
			Tile::SAND,
			Tile::SANDSTONE,
			Tile::SAND);
		generate_flora(Random::one_to_two_fifty_six, Level::DESERT_FLORA);
		break;

	case SNOWY_TAIGA:
		generate_terrain(0.01f, -0.25f, 0.25f, 0.01f,
			Tile::ICE,
			Tile::SNOW,
			Tile::DIRT_WALL,
			Tile::DIRT);
		generate_trees(Random::one_to_eight, Level::COLD_FLORA);
		generate_flora(Random::one_to_two_fifty_six, Level::COLD_FLORA);
		break;
	
	case OCEAN:
		generate_terrain(0.01f, 1.0f, 0.0f, 0.0f,
			Tile::WATER,
			Tile::WATER,
			Tile::WATER,
			Tile::WATER);
		break;
	}

//	for (int i = 0; i < _actors.size(); i++) {
//		_map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]].push_back(_actors[i]);
//	}
	
	_fovmap = new TCODMap(_width, _height);
	
	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}

	std::cout << "LEVEL UPDATED\n";
	std::cout << "Buffer status: " << Actor::get_buffer()->size() << std::endl;
	std::cout << "Size of level: " << _actors.size() << std::endl;
}

void Level::generate_space() {
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			new Tile(x, y, 0, Tile::SPACE);
		}
	}
}

void Level::generate_space_obj(std::uniform_int_distribution<int> r, LEVEL_TYPE type) {
	int numplanets = Random::one_to_eight(Random::generator);
	
	switch (type) {
	case SPACE:
		for (int i = 0; i < _actors.size(); i++) {

			if (r(Random::generator) == 1) {
				new Tile(_actors[i]->get_screen_pos()[0], _actors[i]->get_screen_pos()[1], 0, Tile::DISTANT_STAR);
			}
		}
		
		break;
	
	case SOLAR_SYSTEM:
		
		for (int i = 0; i < _actors.size(); i++) {
			
			if (r(Random::generator) == 1) {
				new Tile(_actors[i]->get_screen_pos()[0], _actors[i]->get_screen_pos()[1], 0, Tile::DISTANT_STAR);
			}
		}
		
		new Tile(_width / 2, _height / 2, 0, Tile::STAR);

		for (int i = 0; i < numplanets; i++) {
			Actor* a = _actors[Random::big_number(Random::generator) % _actors.size()];
			new Planet(a->get_screen_pos()[0], a->get_screen_pos()[1], 0, Planet::TERRA);
		}

		break;
	
	case STAR_SECTOR:
		for (int i = 0; i < _actors.size(); i++) {
			int rnd = r(Random::generator);
			if (rnd == 2) {
				new Tile(_actors[i]->get_screen_pos()[0], _actors[i]->get_screen_pos()[1], 0, Tile::STAR_DUST);
			}
			if (rnd == 1) {
				new SolarSystem(_actors[i]->get_screen_pos()[0], _actors[i]->get_screen_pos()[1], 0);
			}
		}
	
		break;
	
	case GALAXY:
		TCODImage im = TCODImage::TCODImage("Data\\Galaxy1.png");
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				if (im.getPixel(x, y).getValue() > 0) {
					if (r(Random::generator) == 1) {
						new StarSector(x, y, 0);
					}
				}
			}
		}
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				if (im.getPixel(x, y).getValue() > 0) {
					_map[x][y].back()->set_bcolor(Random::one_to_sixty_four(Random::generator), 0, Random::one_to_sixty_four(Random::generator));
				}
			}
		}
		break;
	}
}

void Level::update() {
	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}
	_map.clear();
	_map.resize(_height);
	for (int i = 0; i < _map.size(); i++) {
		_map[i].resize(_width);
	}
	for (int i = 0; i < _actors.size(); i++) {
		_map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]].push_back(_actors[i]);
	}
	std::cout << "LEVEL UPDATED\n";
	std::cout << "Buffer status: " << Actor::get_buffer()->size() << std::endl;
	std::cout << "Size of level: " << _actors.size() << std::endl;
}

void Level::update_tile(int x, int y, int z) {
	std::vector<Actor*> actors = Actor::get_actors(x, y, z);
	_map[x][y].clear();
	for (Actor* a : actors) {
		_fovmap->setProperties(a->get_world_pos()[0], a->get_world_pos()[1], a->is_transparent(), a->is_impassable());
		_map[x][y].push_back(a);
	}
}

void Level::generate_terrain(float frequency, float water_threshold, float terrain_threshold, float beach_size,
	Tile::TILE_TYPE water,
	Tile::TILE_TYPE terrain,
	Tile::TILE_TYPE wall,
	Tile::TILE_TYPE beach) {
	
	_noise.SetFrequency(frequency);
	
	float c;
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			//				new Actor(x, y, 0, 219, TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)), TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)));
			c = static_cast<float>(_noise.GetNoise(x, y));
			if (c < 1.0f && c > terrain_threshold) {
				new Tile(x, y, 0, water);
				continue;
			}
			if (c < terrain_threshold - beach_size && c > water_threshold) {
				new Tile(x, y, 0, terrain);
				continue;
			}
			if (c < terrain_threshold + beach_size && c > water_threshold) {
				new Tile(x, y, 0, beach);
				continue;
			}
			if (c < water_threshold) {
				new Tile(x, y, 0, wall);
				continue;
			}
		}
	}
}

void Level::generate_trees(std::uniform_int_distribution<int> r, Level::GENERATION_FLAG flag) {
	switch (flag) {
	case TEMPERATE_FLORA:
		for (int i = 0; i < _actors.size(); i++) {

			if (_actors.operator[](i)->get_name() == "Grass") {

				if (r(Random::generator) == 1) {
					new Tile(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Tile::TREE);
				}

			}
		}
		break;
	case COLD_FLORA:
		for (int i = 0; i < _actors.size(); i++) {

			if (_actors.operator[](i)->get_name() == "Snow") {

				if (r(Random::generator) == 1) {
					new Tile(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Tile::SNOWY_TREE);
				}

			}
		}
	}
}

void Level::generate_flora(std::uniform_int_distribution<int> r, Level::GENERATION_FLAG flag) {
	switch (flag) {

		case TEMPERATE_FLORA:
			for (int i = 0; i < _actors.size(); i++) {
				if (_actors.operator[](i)->get_name() == "Grass") {

					if (r(Random::generator) == 1) {
						new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::FLOWER);
					}

					if (r(Random::generator) == 1) {
						new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::BUSH);
					}
				}
			}
			break;
		case DESERT_FLORA:
			for (int i = 0; i < _actors.size(); i++) {
				if (_actors.operator[](i)->get_name() == "Sand") {

					if (r(Random::generator) == 1) {
						new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::CACTUS);
					}
				
					if (r(Random::generator) == 1) {
						new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::DEAD_BUSH);
					}
				}
			}
			break;
		case COLD_FLORA:
			for (int i = 0; i < _actors.size(); i++) {
				if (_actors.operator[](i)->get_name() == "Snow") {

					if (r(Random::generator) == 1) {
						new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::SNOW_BUSH);
					}
				}
			}
	}
}

void Level::generate_structures(int num) {
	for (int i = 0; i < num; i++) {
		
		int rnd = 0;
		while (true) {
			rnd = Random::randc(0, _actors.size());
			if (_actors[rnd]->is_impassable()) {
				continue;
			}
			if (_actors[rnd]->get_screen_pos()[0] > _width - 9 || _actors[rnd]->get_screen_pos()[1] > _height - 9) {
				continue;
			}
			break;
		}

		new Structure(_actors[rnd]->get_screen_pos()[0], _actors[rnd]->get_screen_pos()[1], Structure::CABIN);
		std::cout << "Structure generated at: " << _actors[rnd]->get_screen_pos()[0] << " " << _actors[rnd]->get_screen_pos()[1] << std::endl;
	}
}

void Level::save_level_image(std::string path) {
	std::vector<std::vector<TCODColor>> map;
	map.resize(_height);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(_width);
	}
	float h, s, v;
	for (int i = 0; i < _actors.size(); i++) {
		_actors[i]->get_color(&h, &s, &v);
		map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]] = TCODColor(h, s, v);
	}
	
	TCODImage img(_width, _height);

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			img.putPixel(y, x, map[y][x]);
		}
	}

	img.save(path.c_str());
}

void Level::save_level_file(std::string path) {
	TCODZip zip;
	
	int size = _width + _height;
	zip.putInt(size);
	
	for (int i = 0; i < _actors.size(); i++) {
		_actors[i]->serialize(&zip);
	}
	zip.saveToFile(path.c_str());
}

Level* Level::load_level_file(std::string path) {
	TCODZip zip;
	Level* level = new Level;
	
	zip.loadFromFile(path.c_str());

	int size;
	size = zip.getInt();
	
	while (true) {
		std::string typestring = zip.getString();;
		
		if (typestring.size() == 0) {
			break;
		}

		Actor* actor = GameObjects::type_map[typestring]();

		actor->deserialize(&zip);
		level->_actors.push_back(actor);
	}
	
	level->_actors.pop_back();
	level->generate_level(size, Level::NONE);
	return level;
}