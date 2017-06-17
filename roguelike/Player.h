#pragma once
#include "Character.h"
#include "GUI.h"

#include <iostream>


class Player : public Character {
public:
	Player();
	Player(int x, int y, int z, char c, TCODColor color);
	~Player();

	virtual void move(const std::string& dir) override;

	void spawn_player_in_world();

	void spawn_player_in_galaxy();

	void set_info(InfoViewer* info) { _info = info; }
protected:
	InfoViewer* _info = nullptr;;
};

class Dummy : public Player {
public:
	Dummy(int x, int y, int z, char c, TCODColor color);

	~Dummy();

	virtual void move(const std::string& dir) override;

	virtual void draw_mem() override;
};

