#include "Economy.h"
#include "Random.h"

#include <iostream>
#include <string>
#include <algorithm>

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

	std::vector<int> sv = supply_values();
	std::vector<int> dv = demand_values();

	// Make demand go down if supply outnumbers demand
	for (int i = 0; i < sv.size(); i++) {
		if (sv[i] > dv[i] && dv[i] > 0) {
			dv[i]--;
		}
		// Set to zero if negative
		if (sv[i] < 0) {
			sv[i] = 0;
		}
	}

	goods_change(SUPPLY, sv);
	goods_change(DEMAND, dv);
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
			std::cout << "Aborted building building: Insufficient materials\n";
			return false;
		}
	}
	underconstruction_buildings.push_back(b);
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

Economy::Building::Building(Economy* e) : economy(e)
{}

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
	color = TCODColor::blue;
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
	color = TCODColor::red;
	cost.industrial_goods = 500;
}

void Buildings::IndustrialComplex::update() {
	if (economy->supply.workers / std::log(tier + 1) > 1) {
		economy->supply.industrial_goods += Random::randc(std::log(tier + 1) * 500, std::log(tier + 1) * 1000);
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