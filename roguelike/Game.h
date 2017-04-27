#pragma once
#include <libtcod\libtcod.hpp>
#include <iostream>
#include <cmath>
#include "ASCIICodes.h"
#include <vector>
#include "Actor.h"
#include "Player.h"
#include "GameObjects.h"
#include "Input.h"
#include <cstdlib>
#include <algorithm>
#include "Level.h"

class Game
{
public:
	Game();

	~Game();

	void init();

	void start();

	void game_loop();
	
	void on_event(TCOD_event_t e);

	void on_exit();
	
	void update();

private:
	int _screen_width, _screen_height, _num_actors_drawn;
	float _time;
	TCOD_key_t _key;
	TCOD_mouse_t _mouse;
	TCOD_event_t _event;
	Level* _level = new Level;
};


