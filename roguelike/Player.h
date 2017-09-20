#pragma once
#include "Character.h"
#include "GUI.h"
#include "Level.h"

#include <iostream>


class Player : public Character {
public:
	Player();
	Player(int x, int y, int z, char c, TCODColor color);
	~Player();

	virtual void move(const std::string& dir) override;

	virtual void draw() override;

	void spawn_player_in_world();

	void spawn_player(Level::LEVEL_TYPE type);

	void set_info(InfoViewer* info) { _info = info; }

	std::string get_alias() { return _alias; }

	void set_alias(const std::string& str) { _alias = str; }

protected:
	InfoViewer* _info = nullptr;

private:
	std::string _alias = "";
};

class Dummy : public Player {
public:
	Dummy(int x, int y, int z, char c, TCODColor color);

	~Dummy();

	virtual void move(const std::string& dir) override;

	virtual void draw_mem() override;
};

