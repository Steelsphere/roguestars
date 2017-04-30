#include "Player.h"
#include "GameObjects.h"



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
	for (int i = 0; i < buffer->size(); i++) {
		if (buffer->operator[](i)->get_name() == "Grass") {
			this->set_position(buffer->operator[](i)->get_screen_pos()[0], buffer->operator[](i)->get_screen_pos()[1], 0);
			this->set_world_position(buffer->operator[](i)->get_world_pos()[0], buffer->operator[](i)->get_world_pos()[1], 0);
			return;
		}
	}
}