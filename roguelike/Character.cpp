#include "Character.h"
#include "Level.h"
#include "AI.h"

int Character::_playertime = 0;
std::vector<Character*> Character::_chbuf;

Character::Character() {}

Character::Character(int x, int y, int z) : Actor(x, y, z) {
	_impassable = true;
	_transparent = false;
	_action_available = true;
	_chbuf.push_back(this);
}

Character::Character(int x, int y, int z, char c, TCODColor color, const std::string& name, bool notinbuf) : Actor(x, y, z, c, color, TCODColor::black, name) {
	_impassable = true;
	_transparent = false;
	_action_available = true;
	if (!notinbuf) {
		_chbuf.push_back(this);
	}
}

Character::~Character() {
	_chbuf.erase(std::remove(_chbuf.begin(), _chbuf.end(), this), _chbuf.end());
}

void Character::update() {
	if (_action_available && _name != "Player") {
		AI::execute(this, AI::SIMPLE_FOLLOW);
	}
}

void Character::move(const std::string& dir) {
	_selftime += _speed;
	if (_selftime > _playertime) {
//		_action_available = false;
		_selftime = 0;
		return;
	}
	Actor::move(dir);
}

Monster::Monster(int x, int y, int z) : Character(x, y, z) {
	_c = 'M';
	_fcolor = TCODColor::brass;
	_name = "Monster";
}