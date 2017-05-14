#pragma once
#include "Level.h"

class World {
public:
	World();
	~World();

	void generate_world();

	Level* get_current_level() { return _currlevel; }
private:
	Level* _worldarr[512][512];
	Level* _currlevel;
};

