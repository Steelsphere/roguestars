#include "GameObjects.h"
#include "Tile.h"
#include "Item.h"
#include "Player.h"
#include "Hero.h"
#include "Camera.h"

#include <typeinfo>
#include <string>
#include <filesystem>

int GameObjects::screen_width = 125;
int GameObjects::screen_height = 75;
int GameObjects::ticks;

int GameObjects::level_id_to_load = 0;
int GameObjects::new_level_id = 0;
int GameObjects::old_level_id = 0;
int GameObjects::galaxy_size = GameObjects::MEDIUM;

float GameObjects::time = 0.0f;

bool GameObjects::update = true;
bool GameObjects::player_controlled = false;
bool GameObjects::new_turn = false;

Log* GameObjects::log;
Camera* GameObjects::camera;

std::string GameObjects::savegame_path;

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
	{ typeid(Actor).name(), create_actor_instance<Actor> },
	{ typeid(Tile).name(), create_actor_instance<Tile> },
	{ typeid(Item).name(), create_actor_instance<Item> },
	{ typeid(Player).name(), create_actor_instance<Player> },
	{ typeid(StarSector).name(), create_actor_instance<StarSector> },
	{ typeid(SolarSystem).name(), create_actor_instance<SolarSystem> },
	{ typeid(Planet).name(), create_actor_instance<Planet> },
	{ typeid(Biome).name(), create_actor_instance<Biome> },
	{ typeid(Door).name(), create_actor_instance<Door> },
	{ typeid(Hero).name(), create_actor_instance<Hero> },
	{ typeid(Space).name(), create_actor_instance<Space> },
};

bool GameObjects::file_in_filesystem(const std::string& path, const std::string& file) {
	for (auto f : std::experimental::filesystem::recursive_directory_iterator(path)) {
		if (f.path().filename().string() == file) {
			std::cout << "Searched: " << f.path().filename().string() << std::endl;
			return true;
		}
	}
	return false;
}

bool GameObjects::is_directory_empty(const std::string& path) {
	for (auto f : std::experimental::filesystem::directory_iterator(path)) {
		return false;
	}
	return true;
}

int GameObjects::num_files_in_directory(const std::string& path) {
	int num = 0;
	for (auto f : std::experimental::filesystem::directory_iterator(path)) {
		num++;
	}
	return num;
}

Player* GameObjects::find_player(Level* level) {
	for (Actor* i : (*level->get_actors())) {
		if (i->get_name() == "Player") {
			std::cout << "Player found\n";
			return static_cast<Player*>(i);
		}
	}
	throw "Player was not found!\n";
}

std::string GameObjects::get_description(Actor* a) {
	if (a->get_type() == typeid(StarSector).name()) {
		return "An area that stands out within the sea of stars";
	}
	else {
		return "One of the many unsignificant objects in the universe";
	}
}