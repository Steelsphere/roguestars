#include "Hero.h"
#include "Faction.h"
#include "GameObjects.h"

Hero::Hero() {

}

Hero::Hero(int x, int y, Faction* f) : Actor(x, y, 0) {
	_name = TCODNamegen::generate("object");
	_c = '@';
	_fcolor = TCODColor::red;
	_bcolor = TCODColor::black;
	_impassable = false;
	_transparent = false;
	_faction = f;
	_faction->_heroes.push_back(this);
	_action = IDLE;
	_destination = nullptr;
	_moving = false;
	_home = Actor::get_actor(x, y, 0);
	
	std::cout << Actor::get_buffer()->back()->get_name() << std::endl;
	std::cout << "The hero " << _name << " has arrived in " << _faction->_name << std::endl;
	GameObjects::log->message("The hero " + _name + " has arrived in " + _faction->_name, TCODColor::yellow);
}

Hero::~Hero() {

}

void Hero::leave_playarea() {
	for (Actor* a : Actor::get_actors(_world_x, _world_y, 0)) {
		if (a == this) {
			auto vec = (*Actor::get_map())[_world_x][_world_y];
			vec.erase(std::remove(vec.begin(), vec.end(), this));
			_in_playarea = false;
			return;
		}
	}
}

void Hero::join_playarea() {
	(*Actor::get_map())[_world_x][_world_y].push_back(this);
	_in_playarea = true;
}

void Hero::draw() {
	Actor::set_transparent_background();
	Actor::draw();
}

void Hero::simulate(bool fast) {

	// Decide which action to take
	if (_action == IDLE) {
		
		if (Random::one_to_two_fifty_six(Random::generator) == 1) {
			int r = Random::randc(1, NUM_ACTIONS);
			switch (r) {
			case PRESIDING:
				_destination = _home;
				_moving = true;
				std::cout << _name << " is now PRESIDING\n";
				break;
			case ATTACKING:
				for (Actor* a : _faction->get_owned_tiles()) {
					for (Actor* ab : a->get_adjacent_actors_vec()) {
						if (_faction->other_own_tile(ab)) {
							_destination = ab;
							_moving = true;
							std::cout << _name << " is now ATTACKING\n";
							return;
						}
					}
				}
				break;
			case DEFENDING:
				std::vector<Actor*> spawn;
				for (Actor* a : _faction->get_owned_tiles()) {
					if (a->get_name() == "Star Sector") {
						spawn.push_back(a);
					}
				}
				int rdx = Random::randc(0, spawn.size() - 1);
				_destination = spawn[rdx];
				_moving = true;
				std::cout << _name << " is now DEFENDING\n";
				break;
			}
		}
	}
	
	// Move to destination
	if (_moving == true && _destination != nullptr) {
//		if (fast) {
//			_world_x = _destination->get_world_pos()[0];
//			_world_y = _destination->get_world_pos()[1];
//			_action = IDLE;
//			_moving = false;
//			std::cout << _name << " moved to " << _world_x << " " << _world_y << std::endl;
//		}
//		else {
			TCODPath path = TCODPath(Level::get_fov_map(), 1.0f);
			path.compute(_world_x, _world_y, _destination->get_world_pos()[0], _destination->get_world_pos()[1]);
			if (path.size() == 0) {
				_action = IDLE;
				_moving = false;
			}
			else {
				int x, y;
				int dx = _world_x;
				int dy = _world_y;
				path.get(0, &x, &y);
				if (x < dx && y < dy) {
					move("topleft");
				}
				else if (x == dx && y < dy) {
					move("top");
				}
				else if (x > dx && y < dy) {
					move("topright");
				}
				else if (x < dx && y == dy) {
					move("left");
				}
				else if (x > dx && y == dy) {
					move("right");
				}
				else if (x < dx && y > dy) {
					move("bottomleft");
				}
				else if (x == dx && y > dy) {
					move("bottom");
				}
				else if (x > dx && y > dy) {
					move("bottomright");
				}
//				std::cout << _name << " moved to " << _world_x << " " << _world_y << std::endl;
			}
	//	}
	}
	
}