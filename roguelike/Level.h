#pragma once

#include "Actor.h"
#include "FastNoise.h"
#include "Tile.h"

#include <string>
#include <iostream>
#include <vector>

class Level
{
public:
	enum LEVEL_TYPE {
		NONE,
		GRASSLAND,
		FOREST,
		HILLS,
		DESERT,
	};
	
	enum GENERATION_FLAG {
		TEMPERATE_FLORA,
		DESERT_FLORA,
	};
	
	int id;

	Level();
	~Level();

	void generate_level(int size, LEVEL_TYPE type);
	
	std::vector<Actor*>* get_actors() { return &_actors; }

	std::vector<std::vector<std::vector<Actor*>>>* get_actor_map() { return &_map; }

	TCODMap* get_fov_map() { return _fovmap; }

	void generate_terrain(float frequency, float water_threshold, float terrain_threshold, float beach_size,
		Tile::TILE_TYPE water = Tile::WATER, 
		Tile::TILE_TYPE terrain = Tile::GRASS,
		Tile::TILE_TYPE wall = Tile::DIRT_WALL,
		Tile::TILE_TYPE beach = Tile::SAND);

	void generate_trees(std::uniform_int_distribution<int> r);

	void generate_flora(std::uniform_int_distribution<int> r, Level::GENERATION_FLAG flag = Level::TEMPERATE_FLORA);

	void update();

	void save_level_image(const std::string path);

	void save_level_file(const std::string path);

	static Level* load_level_file(const std::string path);
private:
	int _width, _height;
	std::vector<Actor*> _actors;
	std::vector<std::vector<std::vector<Actor*>>> _map;
	TCODMap* _fovmap = nullptr;
	FastNoise _noise;
};
