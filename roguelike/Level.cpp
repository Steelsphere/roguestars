#include "Level.h"
#include <cstdlib>
#include <Ctime>
#include <FastNoise\FastNoise.h>
#include "Tile.h"
#include "Item.h"

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

		//Generate terrain
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				//				new Actor(x, y, 0, 219, TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)), TCODColor(0.0f, 0.0f, static_cast<float>(noise.GetNoise(x, y) * 100)));
				c = static_cast<float>(noise.GetNoise(x, y));
				if (c > 0.25) {
					new Tile(x, y, 0, Tile::WATER);
				}
				else if (c > 0) {
					new Tile(x, y, 0, Tile::GRASS);
				}
				else {
					new Tile(x, y, 0, Tile::DIRT_WALL);
				}
			}
		}

		//Generate trees
		for (int i = 0; i < _actors.size(); i++) {
			if (_actors.operator[](i)->get_name() == "Grass") {
				if (Random::one_to_sixteen(Random::generator) == 1) {
					new Tile(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Tile::TREE);
				}
			}
		}

		//Generate items
		for (int i = 0; i < _actors.size(); i++) {
			if (_actors.operator[](i)->get_name() == "Grass") {
				if (Random::one_to_sixty_four(Random::generator) == 1) {
					new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::FLOWER);
				}
				if (Random::one_to_sixty_four(Random::generator) == 1) {
					new Item(_actors.operator[](i)->get_screen_pos()[0], _actors.operator[](i)->get_screen_pos()[1], 0, Item::BUSH);
				}
			}
		}

		break;
	}

	_fovmap = new TCODMap(w, h);
	int c = 0;

	for (int i = 0; i < _actors.size(); i++) {
		_fovmap->setProperties(_actors[i]->get_world_pos()[0], _actors[i]->get_world_pos()[1], _actors[i]->is_transparent(), !_actors[i]->is_impassable());
	}

	std::cout << "Size of level: " << _actors.size() << std::endl;
}