#pragma once
#include "Level.h"
#include "FastNoise.h"

#include <vector>

class World {
public:
	World();
	
	World(int size);

	~World();

	void generate_world();

	void new_level(int x, int y);

	void save_temperature_map(std::string path);

	void save_terrain_map(std::string path);

	void save_temp_and_terrain_maps(std::string path);

	Level* get_current_level() { return _currlevel; }
private:
	int _width, _height;
	int _numlevels = 0;
	std::vector<std::vector<Level*>> _world;
	Level* _currlevel;
	FastNoise _temp_n;
	FastNoise _terrain_n;
};

