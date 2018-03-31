#pragma once
#define TCOD_SDL2
#include "Level.h"

#include <FastNoise.h>
#include <vector>

class World {
public:
	struct LevelTile {
		Level* level;
		Level::LEVEL_TYPE biome;
		int temp = 0;
		int height = 0;
		float biome_noise = 0.0f;
	};

	World();

	World(int size);

	~World();

	void generate_world();

	void new_level(int x, int y);

	void spawn();

	void generate_temperature();

	void generate_terrain();

	void generate_biome_map();

	void generate_biomes();

	void save_temp_and_terrain_maps(std::string path);

	void save_biome_map(std::string path);

	Level* get_current_level() { return _currlevel; }

	Level* generate_world_map();

	std::pair<int, int> get_current_pos() { return _currpos; }
private:
	std::vector<Level::LEVEL_TYPE> hot_biomes = {
		Level::DESERT,
	};
	std::vector<Level::LEVEL_TYPE> temperate_biomes = {
		Level::FOREST,
		Level::GRASSLAND,
		Level::HILLS,
	};
	std::vector<Level::LEVEL_TYPE> cold_biomes = {
		Level::SNOWY_TAIGA,
	};

	int _width, _height;
	int _numlevels = 0;
	int _temperature = 250;
	std::pair<int, int> _currpos;
	std::vector<std::vector<LevelTile>> _world;
	Level* _currlevel;
	FastNoise _terrain_n;
	FastNoise _biome_n;
};
