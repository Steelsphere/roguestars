#include "Faction.h"
#include "GameObjects.h"

#include <iostream>

std::vector<Faction*> Faction::_factions;

Faction::Faction() {

}

Faction::Faction(int startx, int starty) {
	_factions.push_back(this);
	_capital_tile = Actor::get_actors(startx, starty, 0).back();
	_owned_tiles.push_back(_capital_tile);
	_color = TCODColor(Random::randc(0, 255), Random::randc(0, 255), Random::randc(0, 255));
	
	std::string name = TCODNamegen::generate("object");
	
	if (Random::coinflip(Random::generator) == 1) {
		if (Random::one_to_eight(Random::generator) == 1) {
			name.insert(0, "The ");
		}
		name += suffixes[Random::randc(0, suffixes.size() - 1)];
	}
	else {
		if (Random::one_to_eight(Random::generator) == 1) {
			name.insert(0, " the ");
		}
		name.insert(0, prefixes[Random::randc(0, prefixes.size() - 1)]);
	}
	
	_name = name;
	_fast = true;

	std::cout << "A new nation, " << _name << ", has been founded\n";
	GameObjects::log->message("A new nation, " + _name + ", has been founded", TCODColor::green);
	GameObjects::log->draw(true);
}

Faction::~Faction() {
	_factions.erase(std::remove(_factions.begin(), _factions.end(), this));
	std::cout << "Number of factions: " << _factions.size() << std::endl;
}

void Faction::simulate() {
	if (Random::coinflip(Random::generator) == 1) { 
		// EXPAND
		int offset = _owned_tiles.size() - 1;
		
		std::vector<Actor*> ex_points;
		
		// Determine suitable expansion tiles
		while (true) {
			ex_points = _owned_tiles[offset]->get_adjacent_actors_vec();
			for (int i = ex_points.size() - 1; i >= 0; i--) {
				if (ex_points[i] == nullptr || 
					self_own_tile(ex_points[i]) ||
					ex_points[i]->get_bcolor_obj() == TCODColor::black) {
					ex_points.erase(ex_points.begin() + i);
				}
			}
			if (ex_points.size() != 0) {
				break;
			}
			else {
				offset--;
				if (offset < 0) {
					return;
				}
			}
		}
		
		int idx2 = Random::randc(0, ex_points.size() - 1);
		if (ex_points[idx2] == nullptr) {
			std::cout << "Something went wrong" << std::endl;
			return;
		}
		
		// Remove if another faction owns it
		Faction* owner = who_owns_tile(ex_points[idx2]);
		if (owner != nullptr) {
			owner->remove_tile(ex_points[idx2]);
		}

		_owned_tiles.push_back(ex_points[idx2]);
	}
	else { 
		// FORTIFY
		
		// Create a hero
		if (Random::randc(0, 4000) == 1) {
			std::vector<Actor*> spawn;
			for (Actor* a : _owned_tiles) {
				if (a->get_name() == "Star Sector") {
					spawn.push_back(a);
				}
			}
			int rdx = Random::randc(0, spawn.size() - 1);
			Actor* s = spawn[rdx];
			new Hero(s->get_world_pos()[0], s->get_world_pos()[1], this);
		}
	}

	for (Hero* h : _heroes) {
		h->simulate(_fast);
	}
	
	// Check if dead
	if (!self_own_tile(_capital_tile)) {
		std::cout << _name << " has collapsed!\n";
		GameObjects::log->message(_name + " has collapsed!", TCODColor::red);
		GameObjects::log->draw(true);
		delete this; // RIP
	}
}

bool Faction::any_own_tile(const Actor* t) {
	for (Faction* f : _factions) {
		for (Actor* a : f->get_owned_tiles()) {
			if (t == a) {
				return true;
			}
		}
	}
	return false;
}

bool Faction::self_own_tile(const Actor* t) {
	for (Actor* a : _owned_tiles) {
		if (t == a) {
			return true;
		}
	}
	return false;
}

bool Faction::other_own_tile(const Actor* t) {
	for (Faction* f : _factions) {
		if (f == this) {
			continue;
		}
		for (Actor* a : f->get_owned_tiles()) {
			if (t == a) {
				return true;
			}
		}
	}
	return false;
}

Faction* Faction::who_owns_tile(const Actor* t) {
	for (Faction* f : _factions) {
		for (Actor* a : f->get_owned_tiles()) {
			if (t == a) {
				return f;
			}
		}
	}
	return nullptr;
}

void Faction::remove_tile(const Actor* t) {
	for (int i = 0; i < _owned_tiles.size(); i++) {
		if (t == _owned_tiles[i]) {
			_owned_tiles.erase(_owned_tiles.begin() + i);
			return;
		}
	}
}

void Faction::save_faction_map(const std::string& path, int size) {
	TCODImage img(size / 2, size / 2);
	for (Faction* f : Faction::get_factions()) {
		for (Actor* a : f->get_owned_tiles()) {
			img.putPixel(a->get_world_pos()[0], a->get_world_pos()[1], f->get_color());
		}
	}
	img.save(path.c_str());
}