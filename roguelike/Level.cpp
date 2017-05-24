#include "Level.h"
#include "Tile.h"
#include "Item.h"
#include "GameObjects.h"

#include <fstream>
#include <cstdlib>
#include <Ctime>
#include <string>

Level::Level() {}

Level::~Level() {
	for (int i = 0; i < _actors.size(); i++) {
		delete _actors[i];
	}
	delete _fovmap;
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
	}

	for (int i = 0; i < _actors.size(); i++) {
		_map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]].push_back(_actors[i]);
	}
	
	_fovmap = new TCODMap(_width, _height);
	
	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}

	std::cout << "Buffer status: " << Actor::get_buffer()->size() << std::endl;
	std::cout << "Size of level: " << _actors.size() << std::endl;
}

void Level::update() {
	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}
	_map.clear();
	for (int i = 0; i < _actors.size(); i++) {
		_map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]].push_back(_actors[i]);
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

void Level::generate_trees(std::uniform_int_distribution<int> r) {
	for (int i = 0; i < _actors.size(); i++) {
		
		if (_actors.operator[](i)->get_name() == "Grass") {
			
			if (r(Random::generator) == 1) {
				new Tile(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Tile::TREE);
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