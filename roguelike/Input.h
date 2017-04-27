#pragma once
#include <libtcod\libtcod.hpp>
#include "GameObjects.h"
#include <iostream>

class Input
{
public:
	static void input(TCOD_key_t key);
	static bool calc_key_cooldown();
private:
	static int _cooldown;
	static int _cdfirst;
};

