#include "Character.h"
#include "Level.h"
#include "AI.h"
#include "GameObjects.h"
#include "Item.h"

int Character::_playertime = 0;
std::vector<Character*> Character::_chbuf;

Character::Character() {}

Character::Character(int x, int y, int z) : Actor(x, y, z) {
	_impassable = true;
	_transparent = false;
	_action_available = true;
	_selftime = 0;
	_inventory = std::vector<Item*>();
	_chbuf.push_back(this);
}

Character::Character(int x, int y, int z, char c, TCODColor color, const std::string& name, bool notinbuf) : Actor(x, y, z, c, color, TCODColor::black, name) {
	_impassable = true;
	_transparent = false;
	_action_available = true;
	_selftime = 0;
	_inventory = std::vector<Item*>();
	if (!notinbuf) {
		_chbuf.push_back(this);
	}
}

Character::~Character() {
	_chbuf.erase(std::remove(_chbuf.begin(), _chbuf.end(), this), _chbuf.end());
}

void Character::update() {
	if (_selftime > 0) {
		_selftime--;
		AI::execute(this, AI::SIMPLE_FOLLOW);
		update();
	}
}

void Character::move(const std::string& dir) {
	Actor::move(dir);
}

void Character::add_to_inventory(Item* i) {
	i->set_in_inventory(true);
	_inventory.push_back(i);
}

bool Character::is_item_in_inventory(const std::string& item) {
	if (_inventory.size() != 0) {
		for (int i = 0; i < _inventory.size(); i++) {
			if (_inventory[i]->get_name() == item) {
				return true;
			}
		}
	}
	return false;
}

Monster::Monster(int x, int y, int z) : Character(x, y, z) {
	_c = 'M';
	_fcolor = TCODColor::brass;
	_name = "Monster";
	_speed = 1000;
}
