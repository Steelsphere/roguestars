#pragma once
#define TCOD_SDL2
#include "Actor.h"
#include "Level.h"

class Camera {
public:
	Camera(Actor* following);
	Camera(int x, int y);

	~Camera();

	void update();

	int* get_screen_pos();
	int* get_world_pos();

	void set_world_pos(int x, int y) { _world_x = x; _world_y = y; }

	void set_level(Level* level) { _level = level; }

	void set_following(Actor* actor) { _following = actor; }

private:
	int _screen_x, _screen_y, _screen_z;
	int _world_x, _world_y, _world_z;
	Actor* _following;
	Level* _level;
};
