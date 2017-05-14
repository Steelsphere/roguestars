#include "World.h"
#include "Random.h"
#include "GameObjects.h"
#include "Level.h"

#include <algorithm>

World::World() {

}


World::~World() {

}

void World::generate_world() {
	_worldarr[256][256] = new Level;
	_currlevel = _worldarr[256][256];

	std::shuffle(GameObjects::biomes.begin(), GameObjects::biomes.end(), Random::generator);
	
	_currlevel->generate_level(1024, GameObjects::biomes[0]);
}