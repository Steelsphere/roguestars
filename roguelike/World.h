#pragma once
#include "Level.h"

#include <FastNoise\FastNoise.h>
#include <vector>

class World {
public:
	struct LevelTile {
		Level* level;
		Level::LEVEL_TYPE biome;
		int temp = 0;
		int height = 0;
		float biome_noise = 0;
	};
	
	World();
	
	World(int size);

	~World();

	void generate_world();

	void new_level(int x, int y);

	void generate_temperature();

	void generate_terrain();

	void generate_biome_map();

	void generate_biomes();

	void save_temp_and_terrain_maps(std::string path);

	void save_biome_map(std::string path);

	Level* get_current_level() { return _currlevel; }
private:
	std::vector<Level::LEVEL_TYPE> biomes = {
		Level::GRASSLAND,
		Level::FOREST,
		Level::HILLS,
		Level::DESERT,
		Level::SNOWY_TAIGA,
	};
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
	int temperature = 250;
	std::vector<std::vector<LevelTile>> _world;
	std::vector<float> _diff_bnoise;
	Level* _currlevel;
	FastNoise _terrain_n;
	FastNoise _biome_n;
};

