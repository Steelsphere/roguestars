#include "Faction.h"

#include <iostream>

std::vector<Faction*> Faction::_factions;

Faction::Faction() {

}

Faction::Faction(int startx, int starty) {
	_factions.push_back(this);
	_capital_tile = Actor::get_actors(startx, starty, 0).back();
	_owned_tiles.push_back(_capital_tile);
	_color = TCODColor(Random::randc(0, 255), Random::randc(0, 255), Random::randc(0, 255));
	_name = std::to_string(Random::random(Random::generator));
}

Faction::~Faction() {

}

void Faction::simulate() {
	if (Random::coinflip(Random::generator) == 1) { 
		// EXPAND
		int offset = _owned_tiles.size() - 1;
		
		std::vector<Actor*> ex_points;
		
		while (true) {
			ex_points = _owned_tiles[offset]->get_adjacent_actors_vec();
			for (int i = ex_points.size() - 1; i >= 0; i--) {
				if (ex_points[i] == nullptr || self_own_tile(ex_points[i]->get_world_pos()[0], ex_points[i]->get_world_pos()[1])) {
					ex_points.erase(ex_points.begin() + i);
				}
			}
			if (ex_points.size() != 0) {
				break;
			}
			else {
				offset--;
			}
		}
		int idx2 = Random::randc(0, ex_points.size() - 1);
		if (ex_points[idx2] == nullptr) {
			std::cout << "Something went wrong" << std::endl;
			return;
		}
		_owned_tiles.push_back(ex_points[idx2]);
	}
	else { 
		// FORTIFY

	}
}

bool Faction::does_faction_own_tile(int x, int y) {
	for (Faction* f : _factions) {
		for (Actor* a : f->get_owned_tiles()) {
			if (a->get_world_pos()[0] == x && a->get_world_pos()[1] == y) {
				return true;
			}
		}
	}
	return false;
}

bool Faction::self_own_tile(int x, int y) {
	for (Actor* a : _owned_tiles) {
		if (Actor::get_actor(x, y, 0) == a) {
			return true;
		}
	}
	return false;
}
