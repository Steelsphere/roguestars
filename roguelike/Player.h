#pragma once
#include "Actor.h"
#include <iostream>


class Player : public Actor
{
public:
	Player(int x, int y, int z, char c, TCODColor color);
	
	~Player();

	virtual void move(std::string dir) override;

	void spawn_player_in_world();
};

