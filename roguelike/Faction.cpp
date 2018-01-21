#include "Faction.h"
#include "GameObjects.h"
#include "Economy.h"
#include "Spaceship.h"


#include <iostream>

std::vector<Faction*> Faction::_factions;

Faction::Faction() {

}

Faction::Faction(int startx, int starty) {
	_factions.push_back(this);
	
	StarSector* capital = dynamic_cast<StarSector*>(Actor::get_actors(startx, starty, 0).back());
	
	if (capital == nullptr) {
		abort();
	}
	else {
		_capital_tile = capital;
		_capital_tile->faction = this;
		std::vector<int> newvals;
		for (int i = 0; i < _capital_tile->economy.supply_values().size(); i++) {
			newvals.push_back(Random::randc(10000, 1000000));
		}
		_capital_tile->economy.goods_change(Economy::SUPPLY, newvals);
		_capital_tile->economy.buildings.push_back(new Buildings::FarmingComplex(&_capital_tile->economy));
		_capital_tile->economy.buildings.push_back(new Buildings::MiningComplex(&_capital_tile->economy));
		_capital_tile->economy.buildings.push_back(new Buildings::IndustrialComplex(&_capital_tile->economy));
	}

	_owned_tiles.push_back(_capital_tile);
	_ssv.push_back(_capital_tile);
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

	std::cout << "A new nation, " << _name << ", has been founded\n";
	GameObjects::log->message("A new nation, " + _name + ", has been founded", TCODColor::green);
	GameObjects::log->draw(true);
}

Faction::~Faction() {
	_factions.erase(std::remove(_factions.begin(), _factions.end(), this));
	
	for (Actor* a : _owned_tiles) {
		a->set_bcolor(Random::one_to_sixty_four(Random::generator), 0, Random::one_to_sixty_four(Random::generator));
		if (a->get_type() == typeid(Space).name()) {
			Space* t = dynamic_cast<Space*>(a);
			t->faction = nullptr;
		}
		else if (a->get_type() == typeid(StarSector).name()) {
			StarSector* t = dynamic_cast<StarSector*>(a);
			t->faction = nullptr;
		}
	}

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
				if (ex_points[i] == nullptr) {
					ex_points.erase(ex_points.begin() + i);
				}
				else if (
					self_own_tile(ex_points[i]) ||
					ex_points[i]->get_bcolor_obj() == TCODColor::black ||
					!(ex_points[i]->get_type() == typeid(Space).name() || ex_points[i]->get_type() == typeid(StarSector).name())) {
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

		// Put into star sector container if it is one
		if (ex_points[idx2]->get_type() == typeid(StarSector).name()) {
			StarSector* ss = dynamic_cast<StarSector*>(ex_points[idx2]);
			if (!ss->colonized) {
				ss->colonize(this);
				_ssv.push_back(ss);
				if (_ssv.size() % 50 == 0) {
					GameObjects::log->message(_name + " has reached the " + std::to_string(_ssv.size()) + " number of star sectors colonized milestone", TCODColor::yellow);
					std::cout << _name + " has reached the " + std::to_string(_ssv.size()) + " number of star sectors colonized milestone" << std::endl;
				}
			}
			else {
				ss->faction = this;
			}
		}
		else if (ex_points[idx2]->get_type() == typeid(Space).name()) {
			Space* s = dynamic_cast<Space*>(ex_points[idx2]);
			s->faction = this;
		}

		_owned_tiles.push_back(ex_points[idx2]);
	}
	else { 
		// FORTIFY
		
		// Manage economy
		decide_buildings();
		decide_ships();


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

	for (StarSector* s : _ssv) {
		s->economy.update();
	}

	for (Hero* h : _heroes) {
		h->simulate();
	}
	
	simulate_ships();

	// Check if any other faction is met

	meet_nations();

	// Check if dead
	if (!self_own_tile(_capital_tile)) {
		std::cout << _name << " has collapsed!\n";
		GameObjects::log->message(_name + " has collapsed!", TCODColor::red);
		GameObjects::log->draw(true);
		delete this; // RIP
	}
}

bool Faction::any_own_tile(Actor* t) {
	if (t == nullptr) {
		return nullptr;
	}
	if (t->get_type() == typeid(Space).name()) {
		Space* a = dynamic_cast<Space*>(t);
		if (a->faction != nullptr) {
			return true;
		}
	}
	else if (t->get_type() == typeid(StarSector).name()) {
		StarSector* a = dynamic_cast<StarSector*>(t);
		if (a->faction != nullptr) {
			return true;
		}
	}
	return false;
}

bool Faction::self_own_tile(Actor* t) {
	if (t == nullptr) {
		return nullptr;
	}
	if (t->get_type() == typeid(Space).name()) {
		Space* a = dynamic_cast<Space*>(t);
		if (a->faction == this) {
			return true;
		}
	}
	else if (t->get_type() == typeid(StarSector).name()) {
		StarSector* a = dynamic_cast<StarSector*>(t);
		if (a->faction == this) {
			return true;
		}
	}
	return false;
}

bool Faction::other_own_tile(Actor* t) {
	if (t == nullptr) {
		return nullptr;
	}
	if (t->get_type() == typeid(Space).name()) {
		Space* a = dynamic_cast<Space*>(t);
		if (a->faction != this && a->faction != nullptr) {
			return true;
		}
	}
	else if (t->get_type() == typeid(StarSector).name()) {
		StarSector* a = dynamic_cast<StarSector*>(t);
		if (a->faction != this && a->faction != nullptr) {
			return true;
		}
	}
	return false;
}

Faction* Faction::who_owns_tile(Actor* t) {
	if (t == nullptr) {
		return nullptr;
	}
	if (t->get_type() == typeid(Space).name()) {
		Space* a = dynamic_cast<Space*>(t);
		return a->faction;
	}
	else if (t->get_type() == typeid(StarSector).name()) {
		StarSector* a = dynamic_cast<StarSector*>(t);
		return a->faction;
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

void Faction::reinit_factions() {
	for (Faction* f : Faction::get_factions()) {
		f->_owned_tiles.clear();
		for (Actor* a : (*Actor::get_buffer())) {
			if (a->get_bcolor_obj() == f->get_color()) {
				f->_owned_tiles.push_back(a);
			}
		}
		for (Actor* a : f->_owned_tiles) {
			if (a->get_name() == "Star Sector" && a->get_color_obj() == TCODColor::yellow) {
				f->_capital_tile = dynamic_cast<StarSector*>(a);
			}
		}
	}
}

void Faction::decide_buildings() {
	for (StarSector* ss : _ssv) {
		// Priority 0
		
		int priority1_rand = Random::randc(0, 2);
		int priority3_rand = Random::randc(0, 1);

		if (!ss->economy.has_building("Infrastructure")) {
			if (ss->economy.build_building(new Buildings::Infrastructure(&ss->economy))) {
				std::cout << _name << " has started building Infrastructure in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::Infrastructure(&ss->economy).cost / 4;
			}
		}
		
		// Priority 1
		
		else if (!ss->economy.has_building("Farming Complex") && priority1_rand == 0) {
			if (ss->economy.build_building(new Buildings::FarmingComplex(&ss->economy))) {
				std::cout << _name << " has started building Farming Complex in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::FarmingComplex(&ss->economy).cost / 4;
			}
		}

		else if (!ss->economy.has_building("Mining Complex") && priority1_rand == 1) {
			if (ss->economy.build_building(new Buildings::MiningComplex(&ss->economy))) {
				std::cout << _name << " has started building Mining Complex in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::MiningComplex(&ss->economy).cost / 4;
			}
		}

		else if (!ss->economy.has_building("Industrial Complex") && priority1_rand == 2) {
			if (ss->economy.build_building(new Buildings::IndustrialComplex(&ss->economy))) {
				std::cout << _name << " has started building Industrial Complex in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::IndustrialComplex(&ss->economy).cost / 4;
			}
		}

		// Priority 2
		
		else if (!ss->economy.has_building("Commercial District")) {
			if (ss->economy.build_building(new Buildings::Commercial(&ss->economy))) {
				std::cout << _name << " has started building Commercial District in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::Commercial(&ss->economy).cost / 4;
			}
		}

		// Priority 3

		else if(!ss->economy.has_building("Space Port") && priority3_rand == 0) {
			if (ss->economy.build_building(new Buildings::SpacePort(&ss->economy))) {
				std::cout << _name << " has started building Space Port in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::SpacePort(&ss->economy).cost / 4;
			}
		}
		
		else if (!ss->economy.has_building("Military Industrial Complex") && priority3_rand == 1) {
			if (ss->economy.build_building(new Buildings::MIC(&ss->economy))) {
				std::cout << _name << " has started building Military Industrial Complex in " << ss->alias << std::endl;
			}
			else {
				ss->economy.demand += Buildings::SpacePort(&ss->economy).cost / 4;
			}
		}
	}
}

void Faction::decide_ships() {
	
	// Freighters
	if (Random::randc(1, 4) == 1 && _numfreighters <= FREIGHTER_LIMIT) {
		for (StarSector* ss : _ssv) {
			if (ss->economy.demand > 0) {
				for (StarSector* ss2 : _ssv) {
					if (ss2->economy.supply > ss->economy.demand) {
						Freighter* f = new Freighter(ss2, this);
						if (ss2->economy.build_ship(f, this)) {
							f->route(ss2, ss, ss->economy.demand);
							std::cout << "Freighter created\n";
							_numfreighters++;
						}
					}
				}
			}
		}
	}
	// Scouts 
	
	if (Random::randc(0, 10) == 0 && _numscouts <= SCOUT_LIMIT) {
		_capital_tile->economy.build_ship(new Scout(_capital_tile, this), this);
		std::cout << "Scout created\n";
		_numscouts++;
	} 

	// Warships

	if (Random::randc(0, 100) == 0) {
		_capital_tile->economy.build_ship(new Warship(_capital_tile, this), this);
		std::cout << "Warship created\n";
	}
}

void Faction::simulate_ships() {
	for (Spaceship* s : spaceships) {

		// Freighter management
		if (s->get_type() == typeid(Freighter).name()) {
			Freighter* f = dynamic_cast<Freighter*>(s);
			if (f->action == Freighter::NONE) {
				for (StarSector* target : _ssv) {
					if (target->economy.demand > 0) {
						for (StarSector* donor : _ssv) {
							if (donor == target) {
								continue;
							}
							
							auto v1 = target->economy.supply.get_vals();
							auto v2 = donor->economy.supply.get_vals();
							
							for (int i = 0; i < v1.size(); i++) {
								if (v2[i] > v1[i]) {
									f->route(donor, target, target->economy.demand);
									break;
								}
							}

							if (f->action != Freighter::NONE) {
								break;
							}
						}
					}
					if (f->action != Freighter::NONE) {
						break;
					}
				}
			}
		}
		
		// Scout management
		else if (s->get_type() == typeid(Scout).name()) {
			Scout* sc = dynamic_cast<Scout*>(s);
			if (sc->action == Scout::NONE) {
				sc->action = Scout::SCOUT_BEGIN;
			}
		}

		// Warship management
		else if (s->get_type() == typeid(Warship).name()) {
			Warship* ws = dynamic_cast<Warship*>(s);
			if (ws->action == Warship::NONE) {
				ws->action = Warship::PATROL_BEGIN;
			}
		}

		s->update();
	}
}

void Faction::meet_nations() {
	for (StarSector* s : _ssv) {
		for (Actor* a : s->get_adjacent_actors_vec()) {
			if (a == nullptr) {
				continue;
			}
			else if (a->get_type() == typeid(StarSector).name()) {
				StarSector* b = dynamic_cast<StarSector*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
			else if (a->get_type() == typeid(Spaceship).name()) {
				Spaceship * b = dynamic_cast<Spaceship*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
			else if (a->get_type() == typeid(Space).name()) {
				Space* b = dynamic_cast<Space*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
		}
	}
	for (Spaceship* sp : spaceships) {
		for (Actor* a : sp->get_adjacent_actors_vec()) {
			if (a == nullptr) {
				continue;
			}
			else if (a->get_type() == typeid(StarSector).name()) {
				StarSector* b = dynamic_cast<StarSector*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
			else if (a->get_type() == typeid(Spaceship).name()) {
				Spaceship* b = dynamic_cast<Spaceship*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
			else if (a->get_type() == typeid(Space).name()) {
				Space* b = dynamic_cast<Space*>(a);
				if (b->faction != nullptr && b->faction != this) {
					if (diplomatic_relations.count(b->faction) == 0) {
						discover_nation(b->faction);
					}
				}
			}
		}
	}
}

void Faction::discover_nation(Faction* f) {
	diplomatic_relations[f] = Random::randc(-25, 25);
	f->diplomatic_relations[this] = Random::randc(-25, 25);
	std::cout << _name << " has met " << f->get_name() << " " << diplomatic_relations[f] << " " << f->diplomatic_relations[this] << std::endl;
}