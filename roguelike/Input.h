#pragma once
#include "Actor.h"

#include <libtcod\libtcod.hpp>
#include <iostream>

class Input
{
public:
	enum MODE {
		NORMAL,
		ENTER_TO_CONTINUE,
		NONE,
	};
	
	static void input(TCOD_key_t key);
	
	static void set_input_reciever(Actor* r) { _reciever = r; }
	
	static MODE get_mode() { return _mode; }
	
	static void set_mode(MODE mode) { _mode = mode; }

	static TCOD_key_t get_last_key() { return _lastkey; }

	static void refresh() { _lastkey.pressed = false; }

private:
	static Actor* _reciever;
	static MODE _mode;
	static TCOD_key_t _lastkey;
};

