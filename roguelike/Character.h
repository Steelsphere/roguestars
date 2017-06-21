#pragma once
#include "Actor.h"

class Character : public Actor {
public:
	
	Character();
	Character(int x, int y, int z);
	Character(int x, int y, int z, char c, TCODColor color, const std::string& name = "Character", bool notinbuf = false);
	~Character();

	static std::vector<Character*>* get_chbuff() { return &_chbuf; }

	int get_selftime() { return _selftime; }
	void set_selftime(int s) { _selftime = s; }

	int get_speed() { return _speed; }

	virtual void update();

	virtual void move(const std::string& dir);

protected:
	static int _playertime;
	
	int _health;
	int _speed;
	int _selftime;
	bool _action_available;
	
private:
	static std::vector<Character*> _chbuf;
};

class Monster : public Character {
public:
	Monster(int x, int y, int z);
};

