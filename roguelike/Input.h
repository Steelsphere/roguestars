#pragma once
#include <libtcod\libtcod.hpp>
#include "GameObjects.h"
#include <iostream>

class Input
{
public:
	static void input(TCOD_key_t key);
	static bool calc_key_cooldown();
	static void set_input_reciever(Actor* r) { _reciever = r; }
private:
	static int _cooldown;
	static int _cdfirst;
	static Actor* _reciever;
};

