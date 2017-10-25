#pragma once
#define TCOD_SDL2
#include "Character.h"

class Faction;
class Hero : public Actor {
public:
	enum Action {
		IDLE,
		PRESIDING,
		ATTACKING,
		DEFENDING,
		NUM_ACTIONS,
	};
	
	Hero();
	Hero(int x, int y, Faction* f);
	~Hero();

	void leave_playarea();
	void join_playarea();

	virtual void draw();

	void simulate(bool fast);

private:
	bool _in_playarea;
	Faction* _faction;
	Action _action;
	Actor* _home;
	Actor* _destination;
	bool _moving;
};

