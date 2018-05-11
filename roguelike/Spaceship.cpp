#include "Spaceship.h"
#include "Tile.h"
#include "Faction.h"
#include "Economy.h"
#include "Level.h"


Spaceship::Spaceship(char c, StarSector* s, Faction* f) : Actor(s->get_world_pos()[0], s->get_world_pos()[1], 0, c, TCODColor::white, TCODColor::black, "Ship")  {
	_bcolor = f->get_color();
	faction = f;
}


Spaceship::~Spaceship() {
	delete_actor();
}

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

void Spaceship::serialize(TCODZip* zip) {
	Actor::serialize(zip);
	cost.serialize(zip); // Economy::Goods
//	path.serialize(zip); // std::vector<Actor *>
}

void Spaceship::deserialize(TCODZip* zip) {
	Actor::deserialize(zip);
	cost.deserialize(zip); // Economy::Goods
//	path.deserialize(zip); // std::vector<Actor *>
}

Freighter::Freighter(StarSector* s, Faction* f) : Spaceship('F', s, f) {
	cost.military_goods = 100;
	_fcolor = TCODColor::orange;
}

void Freighter::update() {
	// Loiter
	if (Random::randc(0, 5) == 0) {
		return;
	}
	
	Spaceship::update();
	
	if (action == PICKUP) {
		if (path.size() == 0) {
			for (int i : (load_dest->economy.supply - _willpickup).get_vals()) {
				if (i < 0) {
					action = NONE;
					load_dest = nullptr;
					unload_dest = nullptr;
					return;
				}
			}
			cargo += _willpickup;
			load_dest->economy.supply -= _willpickup;
			action = UNLOAD;
			path_to_location(unload_dest->get_world_pos()[0], unload_dest->get_world_pos()[1]);
			load_dest = nullptr;
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
	
	auto v = g.get_vals();
	for (int i = 0; i < v.size(); i++) {
		if (v[i] > FREIGHTER_MAX_CARGO) {
			v[i] = FREIGHTER_MAX_CARGO;
		}
	}
	g.set_vals(v);

	_willpickup = g;
	
	path_to_location(start->get_world_pos()[0], start->get_world_pos()[1]);
	action = PICKUP;
}

void Freighter::serialize(TCODZip* zip) {
	Spaceship::serialize(zip);
	cargo.serialize(zip); // Economy::Goods
	zip->putInt(action);
	_willpickup.serialize(zip); // Economy::Goods
}

void Freighter::deserialize(TCODZip* zip) {
	Spaceship::deserialize(zip);
	cargo.deserialize(zip); // Economy::Goods
	action = (FREIGHTER_ACTION)zip->getInt();
	_willpickup.deserialize(zip); // Economy::Goods
}

Scout::Scout(StarSector* s, Faction* f) : Spaceship('s', s, f) {
	cost.military_goods = 25;
	_fcolor = TCODColor::blue;
}

void Scout::update() {
	Spaceship::update();
	if (action == SCOUT_BEGIN) {
		int xoff = Random::randc(-1, 1);
		int yoff = Random::randc(-1, 1);
		Actor* a = Actor::get_actor(_world_x, _world_y, 0);
		for (int i = 0; i < Random::randc(5, 50); i++) {
			if (Actor::get_actor(a->get_world_pos()[0] + xoff, a->get_world_pos()[1] + yoff, 0) == nullptr) {
				int xoff = Random::randc(-1, 1);
				int yoff = Random::randc(-1, 1);
			}
			else {
				a = Actor::get_actor(_world_x + xoff, _world_y + yoff, 0);
			}
		}
		path_to_location(a->get_world_pos()[0], a->get_world_pos()[1]);
		action = SCOUTING;
	}
	else if (action == SCOUTING) {
		if (path.size() == 0) {
			action = NONE;
		}
	}
}

void Scout::serialize(TCODZip* zip) {
	Spaceship::serialize(zip);
	zip->putInt(action);
}

void Scout::deserialize(TCODZip* zip) {
	Spaceship::deserialize(zip);
	action = (SCOUT_ACTION)zip->getInt();
}

Warship::Warship(StarSector* s, Faction* f) : Spaceship('M', s, f) {
	cost.military_goods = 100;
	_fcolor = TCODColor::red;
}

void Warship::update() {
	Spaceship::update();
	if (action == PATROL_BEGIN) {
		std::vector<StarSector*> sv = faction->get_ssv();
		
		if (sv.size() == 0) {
			action = NONE;
			return;
		}

		std::vector<int> indices;

		for (int i = 0; i < Random::randc(3, 10); i++) {
			indices.push_back(Random::randc(0, sv.size() - 1));
		}

		_patrol_points.clear();
		for (int i : indices) {
			_patrol_points.push_back(sv[i]);
		}

		action = PATROLLING;
	}
	else if (action == PATROLLING) {
		if (path.size() == 0) {
			if (_patrol_points.size() == 0) {
				action = NONE;
			}
			else {
				path_to_location(_patrol_points[0]->get_world_pos()[0], _patrol_points[0]->get_world_pos()[1]);
				_patrol_points.erase(_patrol_points.begin());
			}
		}
	}
}

void Warship::serialize(TCODZip* zip) {
	Spaceship::serialize(zip);
	zip->putInt(action);
//	_patrol_points.serialize(zip); // std::vector<StarSector *>
}

void Warship::deserialize(TCODZip* zip) {
	Spaceship::deserialize(zip);
	action = (MILSHIP_ACTION)zip->getInt();
//	_patrol_points.deserialize(zip); // std::vector<StarSector *>
}