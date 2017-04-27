#pragma once
#include <vector>
#include "Actor.h"
#include <iostream>


class Level
{
public:
	enum LEVEL_TYPE {
		GRASSLAND,
		FOREST,
		HILLS,
	};
	
	Level();
	~Level();

	void generate_level(int w, int h, LEVEL_TYPE type);
	std::vector<Actor*>* get_actors() { return &_actors; }
private:
	int _width, _height;
	std::vector<Actor*> _actors;
};

