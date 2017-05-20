#pragma once
#include "Actor.h"
#include "Level.h"

class Camera
{
public:
	Camera(Actor* following);
	
	~Camera();

	void update();

	int* get_screen_pos();
	int* get_world_pos();

	void set_level(Level* level) { _level = level; }

	void set_following(Actor* actor) { _following = actor; }

private:
	int _screen_x, _screen_y, _screen_z;
	int _world_x, _world_y, _world_z;
	Actor* _following;
	Level* _level;
};

