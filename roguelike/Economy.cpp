#include "Economy.h"
#include "Random.h"

#include <iostream>
#include <string>

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
		"Industrial Goods",
		"Luxury Goods",
		"Consumer Goods",
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

Economy::Building::Building(Economy* e) : economy(e)
{}

Buildings::FarmingComplex::FarmingComplex(Economy* e) : Economy::Building(e) {
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