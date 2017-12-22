#include "Spaceship.h"
#include "Tile.h"
#include "Faction.h"
#include "Economy.h"
#include "Level.h"


Spaceship::Spaceship(char c, StarSector* s, Faction* f) : Actor(s->get_world_pos()[0], s->get_world_pos()[1], 0, c, TCODColor::white, TCODColor::black, "Ship")  {
	if (!s->economy.has_building("Space Port")) {
		delete this;
		std::cout << "Failed to build ship in " << s->alias << ". Reason: no space port\n";
		return;
	}
	auto a = s->economy.supply.get_vals();
	auto b = cost.get_vals();
	for (int i = 0; i < a.size(); i++) {
		if (a[i] < b[i]) {
			delete this;
			std::cout << "Failed to build ship in " << s->alias << ". Reason: insufficient materials\n";
			return;
		}
		else {
			a[i] -= b[i];
		}
	}
	s->economy.supply.set_vals(a);
	f->spaceships.push_back(this);
	std::cout << "Built ship in " << s->alias << std::endl;
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

	std::cout << "Created path with size " << p.size() << std::endl;

	for (int i = 0; i < p.size(); i++) {
		p.get(i, &dx, &dy);
		path.push_back(Actor::get_actor(dx, dy, 0));
	}

}

void Spaceship::follow_path() {
	if (path.size() == 0) {
		std::cout << "Ship stayed\n";
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
	std::cout << "Ship moved\n";
	path.erase(path.begin());
}

Freighter::Freighter(StarSector* s, Faction* f) : Spaceship('F', s, f) {

}
