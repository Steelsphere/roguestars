#include "Player.h"
#include "GameObjects.h"

Player::Player() {

}

Player::Player(int x, int y, int z, char c, TCODColor fcolor) : Character(x, y, z, c, fcolor)
{
	GameObjects::player_controlled = true;
}


Player::~Player()
{
}


void Player::move(const std::string& dir) {
	int xm = GameObjects::map_dir.at(dir).first;
	int ym = GameObjects::map_dir.at(dir).second;
	
	std::vector<Actor*> checkvec = Actor::get_actors(_world_x + xm, _world_y + ym, _world_z);
	
	if (checkvec.size() == 0) {
		return;
	}

	for (int i = 0; i < checkvec.size(); i++) {
		if (checkvec[i]->is_impassable()) {
			return;
		}
	}
	
	this->set_position(_screen_x + xm, _screen_y + ym, _screen_z);
	this->set_world_position(_world_x + xm, _world_y + ym, _world_z);
	
	if (_info != nullptr) {
		_info->draw();
	}
	
	GameObjects::update = true;
}

void Player::spawn_player_in_world() {
	std::vector<Actor*>* buffer = Actor::get_buffer();
	while (true) {
		int rindex = Random::big_number(Random::generator) % buffer->size();
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

Dummy::Dummy(int x, int y, int z, char c, TCODColor color) : Player(x, y, z, c, color) {
	GameObjects::player_controlled = false;
}

Dummy::~Dummy() {
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