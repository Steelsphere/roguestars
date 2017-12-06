#pragma once
#define TCOD_SDL2
#include "Character.h"
#include "GUI.h"

#include <libtcod.hpp>
#include <iostream>
#include <typeinfo>

class Input
{
public:
	enum MODE {
		NONE,
		NORMAL,
		ESC,
		ENTER_TO_CONTINUE,
		INFO_VIEWER,
		INVENTORY_PANEL,
		MAP,
		CLOSE,
		TEXTBOX,
		MAIN_MENU,
	};
	
	static void input(TCOD_key_t key, TCOD_mouse_t mouse);
	
	static void set_input_reciever(Actor* r) { _reciever = r; }
	
	static MODE get_mode() { return _mode; }
	
	static void set_mode(MODE mode) { _mode = mode; }

	static TCOD_key_t get_last_key() { return _lastkey; }

	static TCOD_mouse_t get_last_mouse() { return _lastmouse; }

	static void refresh() { _lastkey.pressed = false; }

private:
	static Actor* _reciever;
	static MODE _mode;
	static TCOD_key_t _lastkey;
	static TCOD_mouse_t _lastmouse;
};
