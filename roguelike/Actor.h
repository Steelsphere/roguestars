#pragma once

#include "Random.h"

#include <libtcod\libtcod.hpp>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>


class Actor
{
public:
	Actor();
	
	Actor(int x, int y, int z, const std::string& name = "Actor");

	Actor(int x, int y, int z, char c, TCODColor fcolor, TCODColor bcolor, const std::string& name = "Actor");
	
	~Actor();

	virtual void draw();
	virtual void draw_mem();

	int* get_screen_pos();
	int* get_world_pos();

	void get_color(float* h, float* s, float* v);
	
	void set_position(int x, int y, int z);
	void set_world_position(int x, int y, int z);

	char get_char() { return _c; }
	void set_char(char c) { _c = c; }

	std::string get_name() { return _name; }
	void set_name(const std::string& name) { _name = name; }

	std::map<std::string, Actor*> get_adjacent_actors();
	virtual void move(const std::string& dir);
	
	static void set_buffer(std::vector<Actor*>* buffer) { _buffer = buffer; }
	static std::vector<Actor*>* get_buffer() { return _buffer; }
	
	static void set_map(std::vector<std::vector<std::vector<Actor*>>>* map) { _map = map; }
	static std::vector<std::vector<std::vector<Actor*>>>* get_map() { return _map; }

	static Actor* get_actor(int x, int y, int z);
	static std::vector<Actor*> get_actors(int x, int y, int z);

	bool is_impassable() { return _impassable; }
	bool is_transparent() { return _transparent; }
	bool is_memorized() { return _memorized; }
	bool is_in_fov() { return _in_fov; }

	void set_memorization(bool b) { _memorized = b; }
	void set_fov(bool b) { _in_fov = b; }

	virtual void serialize(TCODZip* zip);
	virtual void deserialize(TCODZip* zip);
	
	friend bool operator== (const Actor &a1, const Actor &a2);
	friend bool operator!= (const Actor &a1, const Actor &a2);

	virtual void on_pg_down() {}

protected:
	int _screen_x, _screen_y, _screen_z;
	int _world_x, _world_y, _world_z;
	char _c;
	TCODColor _fcolor;
	TCODColor _bcolor;
	std::string _name;
	bool _impassable;
	bool _transparent;
	bool _memorized;
	bool _in_fov;

private:
	static std::vector<Actor*>* _buffer;
	static std::vector<std::vector<std::vector<Actor*>>>* _map;
};

