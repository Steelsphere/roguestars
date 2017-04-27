#include "Level.h"
#include <cstdlib>
#include <Ctime>
#include <FastNoise\FastNoise.h>


Level::Level()
{
}


Level::~Level()
{
}

void Level::generate_level(int w, int h, LEVEL_TYPE type) {
	_width = w;
	_height = h;
	
	FastNoise noise;
	noise.SetSeed(time(NULL));
	noise.SetNoiseType(FastNoise::PerlinFractal);
	
	switch (type) {
	case GRASSLAND:
		Actor::set_buffer(&_actors);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				new Tile(x, y, 0, Tile::GRASS);
			}
		}
		break;

	case HILLS:
		Actor::set_buffer(&_actors);
		float c = 0;
		int rnd;

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
//				new Actor(x, y, 0, 219, TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)), TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)));
				c = static_cast<float>(noise.GetNoise(x, y));
				if (c < 0) {
					new Tile(x, y, 0, Tile::GRASS);
				}
				else {
					new Tile(x, y, 0, Tile::DIRT_WALL);
				}
			}
		}
		
		for (int i = 0; i < _actors.size(); i++) {
			if (_actors.operator[](i)->get_name() == "Grass") {
				if (Random::one_to_sixteen(Random::generator) == 1) {
					new Tile(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Tile::TREE);
				}
			}
		}
		
		break;
	}
	
	std::cout << "Size of level: " << _actors.size() << std::endl;
}