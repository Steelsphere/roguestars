#pragma once
#include <vector>
#include "Actor.h"
#include <iostream>
#include "FastNoise.h"
#include "Tile.h"

class Level
{
public:
	enum LEVEL_TYPE {
		GRASSLAND,
		FOREST,
		HILLS,
		DESERT,
	};
	
	Level();
	~Level();

	void generate_level(int size, LEVEL_TYPE type);
	
	std::vector<Actor*>* get_actors() { return &_actors; }

	std::vector<std::vector<std::vector<Actor*>>>* get_actor_map() { return &_map; }

	TCODMap* get_fov_map() { return _fovmap; }

	void generate_terrain(float frequency, float water_threshold, float terrain_threshold, 
		Tile::TILE_TYPE water = Tile::WATER, 
		Tile::TILE_TYPE terrain = Tile::GRASS,
		Tile::TILE_TYPE wall = Tile::DIRT_WALL);

	void generate_trees(std::uniform_int_distribution<int> r);

	void generate_items(std::uniform_int_distribution<int> r);

	void update();

	void save_level_image();
private:
	int _width, _height;
	std::vector<Actor*> _actors;
	std::vector<std::vector<std::vector<Actor*>>> _map;
	TCODMap* _fovmap;
	FastNoise _noise;
};

