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
	delete_actor();
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

void Character::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	zip->putInt(_playertime);
	zip->putInt(_health);
	zip->putInt(_speed);
	zip->putInt(_selftime);
	zip->putInt(_action_available);
//	_inventory.serialize(zip); // std::vector<Item *>
//	_bodyparts.serialize(zip); // std::vector<Bodypart>
//	_chbuf.serialize(zip); // static std::vector<Character *>
}

void Character::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	_playertime = zip->getInt();
	_health = zip->getInt();
	_speed = zip->getInt();
	_selftime = zip->getInt();
	_action_available = zip->getInt();
//	_inventory.deserialize(zip); // std::vector<Item *>
//	_bodyparts.deserialize(zip); // std::vector<Bodypart>
//	_chbuf.deserialize(zip); // std::vector<Character
}

Monster::Monster(int x, int y, int z) : Character(x, y, z) {
	_c = 'M';
	_fcolor = TCODColor::brass;
	_name = "Monster";
	_speed = 1000;
}
