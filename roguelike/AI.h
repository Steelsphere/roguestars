#pragma once
#define TCOD_SDL2
#include "Player.h"

class AI {
public:
	enum AI_TYPE {
		NONE,
		SIMPLE_FOLLOW,
	};
	
	AI();
	AI(Character* body, AI_TYPE type);
	~AI();

	static void setup(Player* player);

	static void execute(Character* c, AI_TYPE type);

private:
	static void simple_follow(Character* c);
	static Player* _player;
};

