#include "GameObjects.h"

int GameObjects::screen_width = 100;
int GameObjects::screen_height = 75;

bool GameObjects::update = true;
const std::map<std::string, std::pair<int, int>> GameObjects::map_dir = {
	{ "topleft", std::make_pair(-1, -1) },
	{ "top", std::make_pair(0, -1) },
	{ "topright", std::make_pair(+1, -1) },
	{ "left", std::make_pair(-1, 0) },
	{ "right", std::make_pair(+1, 0) },
	{ "bottomleft", std::make_pair(-1, +1) },
	{ "bottom", std::make_pair(0, +1) },
	{ "bottomright", std::make_pair(+1, +1) },
};
