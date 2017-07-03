#pragma once

#include "Actor.h"
#include "Level.h"

#include <vector>
#include <map>
#include <string>

class GameObjects
{
public:
	static int screen_width, screen_height;

	static int ticks;

	static int level_id_to_load;

	static int new_level_id;

	static float time;

	static bool update;

	static bool player_controlled;

	static bool new_turn;

	static std::string savegame_path;

	static std::map<std::string, std::pair<int, int>> map_dir;

	template <typename T>
	static Actor* create_actor_instance() { return new T; }
	
	static std::map<std::string, Actor*(*)()> type_map;

	static Actor* find_player() {
		for (Actor* i : (*Actor::get_buffer())) {
			if (i->get_name() == "Player") {
				return i;
			}
		}
	}

	static bool file_in_filesystem(const std::string& path, const std::string& file);
};

