#include "AI.h"
#include "Level.h"

Player* AI::_player = nullptr;

AI::AI() {}

AI::AI(Character* body, AI::AI_TYPE type) {}

AI::~AI(){}

void AI::setup(Player* player) {
	_player = player;
}

void AI::execute(Character* c, AI_TYPE type) {
	switch (type) {
	case SIMPLE_FOLLOW:
		simple_follow(c);
		break;
	}
}

void AI::simple_follow(Character* c) {
	int x = 0;
	int y = 0;
	int sposx = c->get_world_pos()[0];
	int sposy = c->get_world_pos()[1];
	
	TCODPath path = TCODPath(Level::get_fov_map(), 1.0f);
	path.compute(sposx, sposy, _player->get_world_pos()[0], _player->get_world_pos()[1]);
	
	if (path.size() == 0) {
		return;
	}
	
	path.get(0, &x, &y);
	
	if (x == NULL && y == NULL) {
		return;
	}

	if (x < sposx && y < sposy) {
		c->move("topleft");
	}
	else if (x == sposx && y < sposy) {
		c->move("top");
	}
	else if (x > sposx && y < sposy) {
		c->move("topright");
	}
	else if (x < sposx && y == sposy) {
		c->move("left");
	}
	else if (x > sposx && y == sposy) {
		c->move("right");
	}
	else if (x < sposx && y > sposy) {
		c->move("bottomleft");
	}
	else if (x == sposx && y > sposy) {
		c->move("bottom");
	}
	else if (x > sposx && y > sposy) {
		c->move("bottomright");
	}
}
