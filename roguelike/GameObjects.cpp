#include "GameObjects.h"
#include "Tile.h"
#include "Item.h"
#include "Player.h"

#include <typeinfo>
#include <string>

int GameObjects::screen_width = 100;
int GameObjects::screen_height = 75;

bool GameObjects::update = true;
bool GameObjects::player_controlled = false;

std::map<std::string, std::pair<int, int>> GameObjects::map_dir = {
	{ "topleft", std::make_pair(-1, -1) },
	{ "top", std::make_pair(0, -1) },
	{ "topright", std::make_pair(+1, -1) },
	{ "left", std::make_pair(-1, 0) },
	{ "right", std::make_pair(+1, 0) },
	{ "bottomleft", std::make_pair(-1, +1) },
	{ "bottom", std::make_pair(0, +1) },
	{ "bottomright", std::make_pair(+1, +1) },
};

std::map<std::string, Actor*(*)()> GameObjects::type_map = {
	{typeid(Actor).name(), create_actor_instance<Actor>},
	{typeid(Tile).name(), create_actor_instance<Tile>},
	{typeid(Item).name(), create_actor_instance<Item>},
	{typeid(Player).name(), create_actor_instance<Player>},
};

std::vector<Level::LEVEL_TYPE> GameObjects::biomes = {
	Level::GRASSLAND,
	Level::FOREST,
	Level::HILLS,
	Level::DESERT,
};