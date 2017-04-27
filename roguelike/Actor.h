#pragma once
#include <libtcod\libtcod.hpp>
#include "ASCIICodes.h"
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include "Random.h"

class Actor
{
public:
	Actor();
	
	Actor(int x, int y, int z, std::string name = "Actor");

	Actor(int x, int y, int z, char c, TCODColor fcolor, TCODColor bcolor, std::string name = "Actor");
	
	~Actor();

	virtual void draw();

	int* get_screen_pos();
	int* get_world_pos();

	void set_position(int x, int y, int z);
	void set_world_position(int x, int y, int z);

	char get_char() { return _c; }
	void set_char(char c) { _c = c; }

	std::string get_name() { return _name; }
	void set_name(std::string name) { _name = name; }

	std::map<std::string, Actor*> get_adjacent_actors();
	virtual void move(std::string dir);
	
	static void set_buffer(std::vector<Actor*>* buffer);
	static std::vector<Actor*>* get_buffer() { return _buffer; }
	
	static Actor* get_actor(int x, int y, int z);
	static std::vector<Actor*> get_actors(int x, int y, int z);

	bool is_impassable() { return _impassable; }

protected:
	int _screen_x, _screen_y, _screen_z;
	int _world_x, _world_y, _world_z;
	char _c;
	TCODColor _fcolor;
	TCODColor _bcolor;
	std::string _name;
	bool _impassable;

private:
	static std::vector<Actor*>* _buffer;
};

class Tile : public Actor {
public:
	enum TILE_TYPE {
		GRASS,
		STONE_WALL,
		DIRT,
		DIRT_WALL,
		WATER,
		TREE,
	};

	Tile(int x, int y, int z, TILE_TYPE type, TCODColor color = TCODColor::black);
};