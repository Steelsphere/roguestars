#include "Spaceship.h"
#include "Tile.h"
#include "Faction.h"
#include "Economy.h"
#include "Level.h"


Spaceship::Spaceship(char c, StarSector* s, Faction* f) : Actor(s->get_world_pos()[0], s->get_world_pos()[1], 0, c, TCODColor::white, TCODColor::black, "Ship")  {
	_bcolor = f->get_color();
}


Spaceship::~Spaceship() {}

void Spaceship::update() {
	follow_path();
}

void Spaceship::path_to_location(int x, int y) {
	path.clear();
	
	int dx = 0;
	int dy = 0;
	
	TCODPath p = TCODPath(Level::get_fov_map());
	p.compute(_world_x, _world_y, x, y);

	for (int i = 0; i < p.size(); i++) {
		p.get(i, &dx, &dy);
		path.push_back(Actor::get_actor(dx, dy, 0));
	}

}

void Spaceship::follow_path() {
	if (path.size() == 0) {
		return;
	}
	if (path[0]->get_world_pos()[0] < _world_x && path[0]->get_world_pos()[1] < _world_y) {
		move("topleft");
	}
	else if (path[0]->get_world_pos()[0] == _world_x && path[0]->get_world_pos()[1] < _world_y) {
		move("top");
	}
	else if (path[0]->get_world_pos()[0] > _world_x && path[0]->get_world_pos()[1] < _world_y) {
		move("topright");
	}
	else if (path[0]->get_world_pos()[0] < _world_x && path[0]->get_world_pos()[1] == _world_y) {
		move("left");
	}
	else if (path[0]->get_world_pos()[0] > _world_x && path[0]->get_world_pos()[1] == _world_y) {
		move("right");
	}
	else if (path[0]->get_world_pos()[0] < _world_x && path[0]->get_world_pos()[1] > _world_y) {
		move("bottomleft");
	}
	else if (path[0]->get_world_pos()[0] == _world_x && path[0]->get_world_pos()[1] > _world_y) {
		move("bottom");
	}
	else if (path[0]->get_world_pos()[0] > _world_x && path[0]->get_world_pos()[1] > _world_y) {
		move("bottomright");
	}
	path.erase(path.begin());
}

Freighter::Freighter(StarSector* s, Faction* f) : Spaceship('F', s, f) {
	cost.military_goods = 25;
	_fcolor = TCODColor::orange;
}

void Freighter::update() {
	Spaceship::update();
	if (action == PICKUP) {
		if (path.size() == 0) {
			if (load_dest->economy.supply - _willpickup >= 0) {
				cargo += _willpickup;
				load_dest->economy.supply -= _willpickup;
				action = UNLOAD;
				path_to_location(unload_dest->get_world_pos()[0], unload_dest->get_world_pos()[1]);
				load_dest = nullptr;
			}
			else {
				std::cout << "Cannot pickup supplies: Insufficient sector supplies\n";
				action = NONE;
				load_dest = nullptr;
				unload_dest = nullptr;
			}
		}
	}
	else if (action == UNLOAD) {
		if (path.size() == 0) {
			if (unload_dest != nullptr) {
				unload_dest->economy.supply += cargo;
				cargo.reset();
				unload_dest = nullptr;
				action = NONE;
			}
			else {
				action = NONE;
			}
		}
	}
}

void Freighter::route(StarSector* start, StarSector* end, Economy::Goods g) {
	load_dest = start;
	unload_dest = end;
	_willpickup = g;
	path_to_location(start->get_world_pos()[0], start->get_world_pos()[1]);
	action = PICKUP;
}
