#include "Player.h"
#include "GameObjects.h"
#include "Game.h"

Player::Player() {

}

Player::Player(int x, int y, int z, char c, TCODColor fcolor) : Character(x, y, z, c, fcolor, "Player", true)
{
	GameObjects::player_controlled = true;
	GameObjects::new_turn = true;
	_speed = 1000;

	Bodypart head = { "Head", 25 };
	Bodypart left_arm = { "Left Arm", 50 };
	Bodypart torso = { "Torso", 100 };
	Bodypart right_arm = { "Right Arm", 50 };
	Bodypart left_leg = { "Left Leg", 50 };
	Bodypart right_leg = { "Right Leg", 50 };

	_bodyparts.push_back(head);
	_bodyparts.push_back(left_arm);
	_bodyparts.push_back(torso);
	_bodyparts.push_back(right_arm);
	_bodyparts.push_back(left_leg);
	_bodyparts.push_back(right_leg);
}


Player::~Player()
{
}


void Player::move(const std::string& dir) {
	int xm = GameObjects::map_dir.at(dir).first;
	int ym = GameObjects::map_dir.at(dir).second;
	
	std::vector<Actor*> checkvec = Actor::get_actors(_world_x + xm, _world_y + ym, _world_z);
	std::vector<std::vector<std::vector<Actor*>>>* invec = Actor::get_map();

	if (checkvec.size() == 0) {
		return;
	}

	for (int i = 0; i < checkvec.size(); i++) {
		if (checkvec[i]->is_impassable()) {
			checkvec[i]->on_collide();
			return;
		}
	}
	
	for (int i = 0; i < (*invec)[_world_x][_world_y].size(); i++) {
		if ((*invec)[_world_x][_world_y][i] == this) {
			(*invec)[_world_x][_world_y].erase((*invec)[_world_x][_world_y].begin() + i);
		}
	}

	(*invec)[_world_x + xm][_world_y + ym].push_back(this);
	
	for (Actor* a : Actor::get_actors(_world_x, _world_y, 0)) {
		Level::get_fov_map()->setProperties(_world_x, _world_y, a->is_transparent(), !a->is_impassable());
	}
	Level::get_fov_map()->setProperties(_world_x + xm, _world_y + ym, _transparent, !_impassable);
	
	Level* level = game.get_current_level();
	level->chunk_delete_actor(this);

	this->set_position(_screen_x + xm, _screen_y + ym, _screen_z);
	this->set_world_position(_world_x + xm, _world_y + ym, _world_z);
	
	level->chunk_add_actor(this);

	if (_info != nullptr) {
		_info->draw();
	}
	
	std::vector<Character*>* ch = Character::get_chbuff();
	for (Character* c : (*ch)) {
		c->set_selftime(_playertime / c->get_speed());
	}
	
	_playertime = _speed;
	GameObjects::update = true;
	GameObjects::new_turn = true;
}

void Player::spawn_player_in_world() {
	std::vector<Actor*>* buffer = Actor::get_buffer();
	while (true) {
		int rindex = Random::randc(0, buffer->size());
		if ((*buffer)[rindex]->is_impassable()) {
			continue;
		}
		int* spos = (*buffer)[rindex]->get_screen_pos();
		int* rpos = (*buffer)[rindex]->get_world_pos();
		set_position(spos[0], spos[1], spos[2]);
		set_world_position(rpos[0], rpos[1], rpos[2]);
		break;
	}
}

void Player::spawn_player(Level::LEVEL_TYPE type) {
	std::string spawntile;
	switch (type) {
	case Level::GALAXY:
		spawntile = "Star Sector";
		break;
	case Level::STAR_SECTOR:
		spawntile = "Star";
		break;
	case Level::SOLAR_SYSTEM:
		spawntile = "Planet";
		break;
	}
	
	std::vector<Actor*>* buffer = Actor::get_buffer();
	while (true) {
		int rindex = Random::randc(0, buffer->size() - 1);
		if (rindex > buffer->size()) {
			continue;
		}
		if ((*buffer)[rindex]->get_name() == spawntile) {
			int* spos = (*buffer)[rindex]->get_screen_pos();
			int* rpos = (*buffer)[rindex]->get_world_pos();
			set_position(spos[0], spos[1], spos[2]);
			set_world_position(rpos[0], rpos[1], rpos[2]);
			break;
		}
	}
}

void Player::draw() {
	Actor::set_transparent_background();
	Actor::draw();
}

void Player::serialize(TCODZip* zip) {
	Character::serialize(zip);
	zip->putString(_alias.c_str());
}

void Player::deserialize(TCODZip* zip) {
	Character::deserialize(zip);
	_alias = zip->getString();
}

Dummy::Dummy(int x, int y, int z, char c, TCODColor color) : Player(x, y, z, c, color) {
	GameObjects::player_controlled = false;
	auto* m = Actor::get_map();
	(*m)[_world_x][_world_y].erase(std::remove((*m)[_world_x][_world_y].begin(), (*m)[_world_x][_world_y].end(), this));
}

Dummy::~Dummy() {
	Character::~Character();
	GameObjects::player_controlled = true;
}

void Dummy::move(const std::string& dir) {
	int xm = GameObjects::map_dir.at(dir).first;
	int ym = GameObjects::map_dir.at(dir).second;

	this->set_position(_screen_x + xm, _screen_y + ym, _screen_z);
	this->set_world_position(_world_x + xm, _world_y + ym, _world_z);
	
	if (_info != nullptr) {
		_info->draw(true);
	}
	
	GameObjects::update = true;
}

void Dummy::draw_mem() {
	TCODConsole::root->putChar(_screen_x, _screen_y, _c);
	TCODConsole::root->setCharForeground(_screen_x, _screen_y, _fcolor);
	TCODConsole::root->setCharBackground(_screen_x, _screen_y, _bcolor);
}

