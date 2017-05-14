#include "Player.h"
#include "GameObjects.h"

Player::Player() {

}

Player::Player(int x, int y, int z, char c, TCODColor fcolor) : Actor(x, y, z, c, fcolor, TCODColor::black)
{
}


Player::~Player()
{
}


void Player::move(std::string dir) {
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
//	std::cout << "Player position: " << _world_x << " " << _world_y << std::endl;
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