#include "Level.h"
#include "Tile.h"
#include "Item.h"
#include "GameObjects.h"

#include <fstream>
#include <cstdlib>
#include <Ctime>
#include <string>

Level::Level()
{
}


Level::~Level()
{
}

void Level::generate_level(int size, LEVEL_TYPE type) {
	_width = size / 2;
	_height = size / 2;

	_noise.SetSeed(time(NULL));
	_noise.SetNoiseType(FastNoise::PerlinFractal);
	
	Actor::set_buffer(&_actors);
	
	switch (type) {
	
	case GRASSLAND:
		generate_terrain(0.01f, -0.25f, 0.25f);
		generate_trees(Random::one_to_one_twenty_eight);
		generate_items(Random::one_to_one_twenty_eight);
		
		break;
	
	case HILLS:
		generate_terrain(0.01f, 0.0f, 0.25f);
		generate_trees(Random::one_to_sixteen);
		generate_items(Random::one_to_sixty_four);

		break;
	
	case FOREST:
		generate_terrain(0.01f, -0.25f, 0.25f);
		generate_trees(Random::one_to_eight);
		generate_items(Random::one_to_one_twenty_eight);

		break;
	
	case DESERT:
		generate_terrain(0.01f, -0.25f, 0.25f, Tile::SAND, Tile::SAND);
		
		break;
	}

	_fovmap = new TCODMap(_width, _height);
	
	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}

	_map.resize(_height);
	
	for (int i = 0; i < _width; i++) {
		_map[i].resize(_width);
	}

	for (int i = 0; i < _actors.size(); i++) {
		_map[_actors[i]->get_world_pos()[0]][_actors[i]->get_world_pos()[1]].push_back(_actors[i]);
	}

	Actor::set_map(&_map); 
	
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

void Level::generate_terrain(float frequency, float water_threshold, float terrain_threshold,
	Tile::TILE_TYPE water,
	Tile::TILE_TYPE terrain,
	Tile::TILE_TYPE wall) {
	
	_noise.SetFrequency(frequency);
	
	float c;
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			//				new Actor(x, y, 0, 219, TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)), TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)));
			c = static_cast<float>(_noise.GetNoise(x, y));
			if (c < 1.0f && c > terrain_threshold) {
				new Tile(x, y, 0, water);
			}
			if (c < terrain_threshold && c > water_threshold) {
				new Tile(x, y, 0, terrain);
			}
			if (c < water_threshold) {
				new Tile(x, y, 0, wall);
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

void Level::generate_items(std::uniform_int_distribution<int> r) {
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
}

void Level::save_level_image() {
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

	img.save("level.png");
}

void Level::save_level_file(std::string path) {
	std::ofstream os;
	os.open(path, std::ios::binary);
	
	int size = _width + _height;
	os.write(reinterpret_cast<char*>(&size), sizeof(size));
	
	for (int i = 0; i < _actors.size(); i++) {
		_actors[i]->serialize(&os);
	}
}

Level* Level::load_level_file(std::string path) {
	std::ifstream is;
	Level* level = new Level;
	int size;
	
	is.open(path, std::ios::binary);
	
	is.read(reinterpret_cast<char*>(&size), sizeof(size));
	
	std::string typestring;
	
	while (!is.eof()) {
		is.read(reinterpret_cast<char*>(&typestring), sizeof(typestring));
		Actor* actor = GameObjects::type_map[typestring]();

		Actor::deserialize(&is, actor);
		level->_actors.push_back(actor);
	}
	
	level->_actors.pop_back();
	level->generate_level(size, Level::NONE);
	return level;
}