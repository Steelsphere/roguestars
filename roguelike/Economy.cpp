#include "Economy.h"
#include "Random.h"
#include "Spaceship.h"
#include "Faction.h"
#include "GameObjects.h"
#include "Serializer.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

Economy::~Economy() {
	for (Building* b : buildings) {
		delete b;
	}
}

std::vector<int> Economy::supply_values() {
	std::vector<int> v;
	v.push_back(supply.food);
	v.push_back(supply.water);
	v.push_back(supply.air);
	v.push_back(supply.consumer_goods);
	v.push_back(supply.industrial_goods);
	v.push_back(supply.luxury_goods);
	v.push_back(supply.military_goods);
	v.push_back(supply.minerals);
	v.push_back(supply.workers);
	return v;
}

std::vector<int> Economy::demand_values() {
	std::vector<int> v;
	v.push_back(demand.food);
	v.push_back(demand.water);
	v.push_back(demand.air);
	v.push_back(demand.consumer_goods);
	v.push_back(demand.industrial_goods);
	v.push_back(demand.luxury_goods);
	v.push_back(demand.military_goods);
	v.push_back(demand.minerals);
	v.push_back(demand.workers);
	return v;
}

void Economy::goods_change(SUPPLY_TYPES type, std::vector<int>& v) {
	if (type == SUPPLY) {
		supply.food = v[0];
		supply.water = v[1];
		supply.air = v[2];
		supply.consumer_goods = v[3];
		supply.industrial_goods = v[4];
		supply.luxury_goods = v[5];
		supply.military_goods = v[6];
		supply.minerals = v[7];
		supply.workers = v[8];
	}
	else if (type == DEMAND) {
		demand.food = v[0];
		demand.water = v[1];
		demand.air = v[2];
		demand.consumer_goods = v[3];
		demand.industrial_goods = v[4];
		demand.luxury_goods = v[5];
		demand.military_goods = v[6];
		demand.minerals = v[7];
		demand.workers = v[8];
	}
}

bool operator== (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] != a2v[i]) {
			return false;
		}
	}
	return true;
}

bool operator!= (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] == a2v[i]) {
			return true;
		}
	}
	return false;
}

Economy::Goods operator+ (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		a1v[i] += a2v[i];
	}
	Economy::Goods g;
	g.set_vals(a1v);
	return g;
}

Economy::Goods operator- (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		a1v[i] -= a2v[i];
	}
	Economy::Goods g;
	g.set_vals(a1v);
	return g;
}

Economy::Goods operator* (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		a1v[i] *= a2v[i];
	}
	Economy::Goods g;
	g.set_vals(a1v);
	return g;
}

Economy::Goods operator/ (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		a1v[i] /= a2v[i];
	}
	Economy::Goods g;
	g.set_vals(a1v);
	return g;
}

void operator+= (Economy::Goods &a1, Economy::Goods &a2) {
	a1 = a1 + a2;
}

void operator-= (Economy::Goods &a1, Economy::Goods &a2) {
	a1 = a1 - a2;
}

void operator*= (Economy::Goods &a1, Economy::Goods &a2) {
	a1 = a1 * a2;
}

void operator/= (Economy::Goods &a1, Economy::Goods &a2) {
	a1 = a1 / a2;
}

bool operator< (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] >= a2v[i]) {
			return false;
		}
	}
	return true;
}

bool operator<= (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] > a2v[i]) {
			return false;
		}
	}
	return true;
}

bool operator> (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] <= a2v[i]) {
			return false;
		}
	}
	return true;
}

bool operator>= (Economy::Goods &a1, Economy::Goods &a2) {
	auto a1v = a1.get_vals();
	auto a2v = a2.get_vals();
	for (int i = 0; i < a1v.size(); i++) {
		if (a1v[i] < a2v[i]) {
			return false;
		}
	}
	return true;
}


void Economy::update() {
	// Change the economy dynamically
	if (supply.workers > supply.food && supply.workers > 0) {
		demand.food++; 
		supply.workers -= Random::randc(0, 10);
		if (supply.workers < 0) {
			supply.workers = 0;
		}
	}
	if (supply.workers > supply.water && supply.workers > 0) {
		demand.water++;
		supply.workers -= Random::randc(0, 100);
		if (supply.workers < 0) {
			supply.workers = 0;
		}
	}
	

	// Workers use supplies
	if (supply.food - supply.workers / 1000 <= 0) {
		supply.food = 0;
	}
	else {
		supply.food -= supply.workers / 1000;
	}
	
	if (supply.water - supply.workers / 500 <= 0) {
		supply.water = 0;
	}
	else {
		supply.water -= supply.workers / 500;
	}
	
	// Update buildings
	for (Building* b : buildings) {
		b->update();
	}
	construct_buildings();

	std::vector<int> sv = supply.get_vals();
	std::vector<int> dv = demand.get_vals();

	// Make demand go down
	for (int i = 0; i < sv.size(); i++) {
		dv[i] -= (sv[i] / 6) * sv[i];
		
		// Constraints
		if (sv[i] < 0) {
			sv[i] = 0;
		}
		if (dv[i] < 0) {
			dv[i] = 0;
		}
		if (sv[i] > MAX_GVAL) {
			sv[i] = MAX_GVAL;
		}
		if (dv[i] > MAX_GVAL) {
			dv[i] = MAX_GVAL;
		}
	}

	supply.set_vals(sv);
	demand.set_vals(dv);

	// Update history
	supply_history.push_back(supply);
	demand_history.push_back(demand);
	if (supply_history.size() > 5) {
		supply_history.erase(supply_history.begin());
	}
	if (demand_history.size() > 5) {
		demand_history.erase(demand_history.begin());
	}
}

void Economy::print_values() {
	std::string vals[] = {
		"Food",
		"Water",
		"Air",
		"Consumer Goods",
		"Industrial Goods",
		"Luxury Goods",
		"Military Goods",
		"Minerals",
		"Workers"
	};
	
	std::cout << "-----SUPPLY-----\n";
	std::vector<int> sv = supply_values();
	std::vector<int> dv = demand_values();
	for (int i = 0; i < sv.size(); i++) {
		std::cout << vals[i] + " " + std::to_string(sv[i]) << "\n";
	}
	std::cout << "-----DEMAND-----\n";
	for (int i = 0; i < dv.size(); i++) {
		std::cout << vals[i] + " " + std::to_string(dv[i]) << "\n";
	}
}

bool Economy::build_building(Building* b) {
	auto s = supply.get_vals();
	auto c = b->cost.get_vals();
	for (int i = 0; i < supply.get_vals().size(); i++) {
		if (s[i] < c[i]) {
		//	std::cout << "Aborted building building: Insufficient materials\n";
			delete b;
			return false;
		}
	}
	underconstruction_buildings.push_back(b);
	return true;
}

bool Economy::build_ship(Spaceship* s, Faction* f) {
	if (!has_building("Space Port")) {
		delete s;
		std::cout << "Failed to build ship. Reason: no space port\n";
		return false;
	}
	auto a = supply.get_vals();
	auto b = s->cost.get_vals();
	for (int i = 0; i < a.size(); i++) {
		if (a[i] < b[i]) {
			delete s;
			std::cout << "Failed to build ship in. Reason: insufficient materials\n";
			return false;
		}
		else {
			a[i] -= b[i];
		}
	}
	supply.set_vals(a);
	f->spaceships.push_back(s);
	std::cout << "Built ship\n";
	return true;
}

void Economy::construct_buildings() {
	for (Building* b : underconstruction_buildings) {
		std::vector<int> in = b->cost.get_vals();
		
		// Complete the building if done
		int sum = 0;
		for (int i : in) {
			sum += i;
		}
		if (sum == 0) {
			std::cout << "Completed " << b->name << std::endl;
			buildings.push_back(b);
			underconstruction_buildings.erase(std::remove(underconstruction_buildings.begin(), underconstruction_buildings.end(), b));
			break; // Stuff breaks if you don't break here
		}
		
		// Subtract cost from 20% of worker count
		std::vector<int> out = in;
		for (int i = 0; i < out.size(); i++) {
			if (out[i] != 0) {
				
				// Gotta remember to subtract from economy
				if (supply.get_vals()[i] - (int)std::ceil(supply.workers * 0.2) > 0) {
					out[i] -= (int)std::ceil(supply.workers * 0.2);
					
					std::vector<int> econ_out = supply.get_vals();
					econ_out[i] -= (int)std::ceil(supply.workers * 0.2);

					supply.set_vals(econ_out);
				}
				
				if (out[i] < 0) {
					out[i] = 0;
				}
			}
		}
		b->cost.set_vals(out);
	}
}

bool Economy::has_building(const std::string& name) {
	for (Building* b : buildings) {
		if (b->name == name)
			return true;
	}
	for (Building* ub : underconstruction_buildings) {
		if (ub->name == name)
			return true;
	}
	return false;
}

Economy::Goods Economy::trend(Economy::SUPPLY_TYPES type) {
	if (type == Economy::SUPPLY) {
		if (supply_history.size() == 0) {
			return Goods();
		}
		else {
			return (supply - supply_history.front());
		}
	}
	else if (type == Economy::DEMAND) {
		if (demand_history.size() == 0) {
			return Goods();
		}
		else {
			return (demand - demand_history.front());
		}
	}
}

void Economy::serialize(TCODZip* zip) {
	supply.serialize(zip); // Goods
	demand.serialize(zip); // demand
	Serializer::serialize_vector(zip, Serializer::pointer_vector_to_reference(buildings));
	Serializer::serialize_vector(zip, supply_history);
	Serializer::serialize_vector(zip, demand_history);
}

void Economy::deserialize(TCODZip* zip) {
	supply.deserialize(zip); // Goods
	demand.deserialize(zip); // demand
	buildings = Serializer::deserialize_vector<Building>(zip, true);
	for (auto* b : buildings) {
		b->economy = this;
	}
	supply_history = Serializer::deserialize_vector<Goods>(zip);
	demand_history = Serializer::deserialize_vector<Goods>(zip);
}

Economy::Building::Building(Economy* e) : economy(e)
{}

void Economy::Building::serialize(TCODZip* zip) {
	cost.serialize(zip);
	zip->putInt(tier);
	zip->putString(name.c_str());
	zip->putString(initial.c_str());
	zip->putColor(&color);
}

void Economy::Building::deserialize(TCODZip* zip) {
	cost.deserialize(zip);
	tier = zip->getInt();
	name = zip->getString();
	initial = zip->getString();
	color = zip->getColor();
}

Buildings::FarmingComplex::FarmingComplex(Economy* e) : Economy::Building(e) {
	name = "Farming Complex";
	initial = "FC";
	color = TCODColor::green;
	cost.industrial_goods = 500;
}

void Buildings::FarmingComplex::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		economy->supply.food += Random::randc(std::log(tier + 1) * 500, std::log(tier + 1) * 1000);
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

Buildings::MiningComplex::MiningComplex(Economy* e) : Economy::Building(e) {
	name = "Mining Complex";
	initial = "MC";
	color = TCODColor::orange;
	cost.industrial_goods = 500;
}

void Buildings::MiningComplex::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		economy->supply.minerals += Random::randc(std::log(tier + 1) * 500, std::log(tier + 1) * 1000);
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

Buildings::IndustrialComplex::IndustrialComplex(Economy* e) : Economy::Building(e) {
	name = "Industrial Complex";
	initial = "IC";
	color = TCODColor::lightAmber;
	cost.industrial_goods = 500;
}

void Buildings::IndustrialComplex::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		int minus = Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		if (economy->supply.minerals - minus > 0) {
			economy->supply.industrial_goods += Random::randc(std::log(tier + 1) * 500, std::log(tier + 1) * 1000);
			economy->supply.minerals -= minus;
		}
		else {
			economy->demand.minerals++;
		}
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

Buildings::Infrastructure::Infrastructure(Economy* e) : Economy::Building(e) {
	name = "Infrastructure";
	initial = "IF";
	color = TCODColor::grey;
	cost.industrial_goods = 250;
	cost.luxury_goods = 250;
	cost.consumer_goods = 250;
}

void Buildings::Infrastructure::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		economy->supply.food += Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		economy->supply.water += Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		economy->supply.air += Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		economy->supply.industrial_goods += Random::randc(std::log(tier + 1) * 5, std::log(tier + 1) * 10);
		economy->supply.luxury_goods += Random::randc(std::log(tier + 1) * 5, std::log(tier + 1) * 10);
		economy->supply.consumer_goods += Random::randc(std::log(tier + 1) * 5, std::log(tier + 1) * 10);
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

Buildings::SpacePort::SpacePort(Economy* e) : Economy::Building(e) {
	name = "Space Port";
	initial = "SP";
	color = TCODColor::white;
	cost.industrial_goods = 500;
	cost.military_goods = 250;
}

void Buildings::SpacePort::update() {

}

Buildings::MIC::MIC(Economy* e) : Economy::Building(e) {
	name = "Military Industrial Complex";
	initial = "MIC";
	color = TCODColor::red;
	cost.industrial_goods = 500;
	cost.military_goods = 500;
}

void Buildings::MIC::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		int minus = Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		if (economy->supply.industrial_goods - minus > 0) {
			economy->supply.military_goods += Random::randc(std::log(tier + 1) * 750, std::log(tier + 1) * 1250);
			economy->supply.industrial_goods -= minus;
		}
		else {
			economy->demand.industrial_goods++;
		}
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

Buildings::Commercial::Commercial(Economy* e) : Economy::Building(e) {
	name = "Commercial District";
	initial = "C";
	color = TCODColor::blue;
	cost.industrial_goods = 500;
}

void Buildings::Commercial::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		int minus = Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
		if (economy->supply.industrial_goods - minus > 0) {
			economy->supply.consumer_goods += Random::randc(std::log(tier + 1) * 250, std::log(tier + 1) * 500);
			economy->supply.luxury_goods += Random::randc(std::log(tier + 1) * 100, std::log(tier + 1) * 200);
			economy->supply.industrial_goods -= minus;
		}
		else {
			economy->demand.industrial_goods++;
		}
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}