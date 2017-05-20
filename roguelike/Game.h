#pragma once

#include "Actor.h"
#include "Player.h"
#include "GameObjects.h"
#include "Level.h"
#include "Camera.h"
#include "GUI.h"
#include "World.h"

#include <cstdlib>
#include <algorithm>
#include <libtcod\libtcod.hpp>
#include <iostream>
#include <cmath>
#include <memory>

class Game
{
public:
	Game();

	~Game();

	void init();

	void start();

	void exit_game();
	
	void startup_new_game();

	void startup_load_game();

	inline void new_main_menu();

	inline void destroy_main_menu();

	inline void new_esc_menu();

	inline void destroy_esc_menu();

	void destroy_garbage();

	void new_info_viewer();

	void destroy_info_viewer();

private:
	void game_loop();

	void on_event(TCOD_event_t e);

	void game_event();

	void update();

	void update_gui(bool all = false);
	
	int _screen_width, _screen_height, _num_actors_drawn, _num_updates;
	float _time;
	TCOD_key_t _key;
	TCOD_mouse_t _mouse;
	TCOD_event_t _event;
	
	Level* _level = nullptr;
	Player* _player = nullptr;
	Player* _dummy = nullptr;
	Camera* _camera = nullptr;
	Log* _log = nullptr;
	Status* _status = nullptr;
	World* _world = nullptr;
	MainMenu* _MainMenu = nullptr;
	ESCMenu* _ESCMenu = nullptr;
	InfoViewer* _info_viewer = nullptr;
};
