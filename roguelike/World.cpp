#include "World.h"
#include "Random.h"
#include "GameObjects.h"
#include "Level.h"

#include <algorithm>
#include <ctime>

World::World() {

}

World::World(int size) 
	: _width(size/2), _height(size/2) {
	_world.resize(_height);
	for (int i = 0; i < _width; i++) {
		_world[i].resize(_width);
	}
}

World::~World() {

}

void World::generate_world() {
	_temp_n.SetSeed(Random::big_number(Random::generator));
	_temp_n.SetNoiseType(FastNoise::CubicFractal);
	_terrain_n.SetSeed(Random::big_number(Random::generator));
	_terrain_n.SetNoiseType(FastNoise::CubicFractal);
	
	generate_temperature();
	generate_terrain();
	
	new_level(256, 256);
	save_temp_and_terrain_maps("Data\\World.png");
}

void World::new_level(int x, int y) {
	_world[x][y].level = new Level;
	_currlevel = _world[x][y].level;

	std::shuffle(GameObjects::biomes.begin(), GameObjects::biomes.end(), Random::generator);

	_currlevel->generate_level(1024, GameObjects::biomes[0]);
	_currlevel->id = _numlevels++;
}

void World::generate_temperature() {
	int tmp = temperature;
	for (int yup = _height / 2, ydn = _height / 2; yup > 0 && ydn < _height; yup--, ydn++) {
		for (int x = 0; x < _width; x++) {
			_world[x][yup].temp = tmp;
		}
		for (int x = 0; x < _width; x++) {
			_world[x][ydn].temp = tmp;
		}
		tmp--;
		if (tmp == 0) {
			break;
		}
	}
}

void World::generate_terrain() {
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_world[x][y].height = static_cast<int>(_terrain_n.GetNoise(x, y) * 255);
		}
	}
}

void World::save_temperature_map(std::string path) {
	std::vector<std::vector<TCODColor>> map;
	map.resize(_height);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(_width);
	}

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			map[x][y] = TCODColor(_world[x][y].temp, 0, 0);
		}
	}

	TCODImage img(_width, _height);

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _width; y++) {
			img.putPixel(x, y, map[x][y]);
		}
	}
	
	img.save(path.c_str());
}

void World::save_terrain_map(std::string path) {
	std::vector<std::vector<TCODColor>> map;
	map.resize(_height);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(_width);
	}

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			map[x][y] = TCODColor(0, static_cast<int>(_terrain_n.GetNoise(x, y) * 255), 0);
		}
	}

	TCODImage img(_width, _height);

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _width; y++) {
			img.putPixel(x, y, map[x][y]);
		}
	}

	img.save(path.c_str());
}

void World::save_temp_and_terrain_maps(std::string path) {
	std::vector<std::vector<TCODColor>> map;
	map.resize(_height);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(_width);
	}

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			if (_world[x][y].temp > 255) {
			map[x][y] = TCODColor(255, static_cast<int>(_terrain_n.GetNoise(x, y) * 255), 0);
			continue;
			}
			map[x][y] = TCODColor(_world[x][y].temp, static_cast<int>(_terrain_n.GetNoise(x, y) * 255), 0);
		}
	}

	TCODImage img(_width, _height);

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _width; y++) {
			img.putPixel(x, y, map[x][y]);
		}
	}

	img.save(path.c_str());
}