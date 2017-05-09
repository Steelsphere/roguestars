#pragma once

#include "Actor.h"

#include <vector>
#include <map>
#include <string>


class GameObjects
{
public:
	static int screen_width, screen_height;
	
	static bool update;
	
	static std::map<std::string, std::pair<int, int>> map_dir;

	template <typename T>
	static Actor* create_actor_instance() { return new T; }
	
	static std::map<std::string, Actor*(*)()> type_map;

};

