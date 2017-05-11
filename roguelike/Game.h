#pragma once

#include "Actor.h"
#include "Player.h"
#include "GameObjects.h"
#include "Input.h"
#include "Level.h"
#include "GUI.h"
#include "Camera.h"

#include <cstdlib>
#include <algorithm>
#include <libtcod\libtcod.hpp>
#include <iostream>
#include <cmath>

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

	void update_gui();
private:
	int _screen_width, _screen_height, _num_actors_drawn, _num_updates;
	float _time;
	TCOD_key_t _key;
	TCOD_mouse_t _mouse;
	TCOD_event_t _event;
	Level* _level;
	Player* _player;
	Camera* _camera;
	Log* _log;
	Status* _status;
};


