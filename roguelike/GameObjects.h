#pragma once
#include <vector>
#include "Actor.h"
#include "Player.h"
#include "Camera.h"
#include <map>
#include <string>

class GameObjects
{
public:
	static int screen_width, screen_height;
	
	static bool update;
	
	static std::vector<Actor*> actors;
	
	static Player* player;
	
	static Camera* camera;

	static const std::map<std::string, std::pair<int, int>> map_dir;
};

