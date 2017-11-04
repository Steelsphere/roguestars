#include "Economy.h"
#include "Random.h"

#include <iostream>

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
	std::vector<int> sv = supply_values();
	std::vector<int> dv = demand_values();

	// Make demand go down if supply outnumbers demand
	bool modified = false;
	for (int i = 0; i < sv.size(); i++) {
		if (sv[i] > dv[i]) {
			dv[i]--;
			modified = true;
		}
	}

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
		supply.workers -= Random::randc(0, 10);
		if (supply.workers < 0) {
			supply.workers = 0;
		}
	}
	
	supply.food -= supply.workers / 10;
	supply.water -= supply.workers / 5;

	if (modified) {
		goods_change(SUPPLY, sv);
		goods_change(DEMAND, dv);
	}
}

void Economy::print_values() {
	std::cout << "SUPPLY\n";
	std::vector<int> sv = supply_values();
	std::vector<int> dv = demand_values();
	for (int i = 0; i < sv.size(); i++) {
		std::cout << sv[i] << "\n";
	}
	std::cout << "DEMAND\n";
	for (int i = 0; i < dv.size(); i++) {
		std::cout << dv[i] << "\n";
	}
}

Economy::Building::Building(Economy* e) : economy(e)
{}

void Buildings::FarmingComplex::update() {
	if (economy->supply.workers / 512 > 1) {
		economy->supply.food += Random::randc(0, 10);
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}

void Buildings::MiningComplex::update() {
	if (economy->supply.workers / 512 > 1) {
		economy->supply.minerals += Random::randc(0, 10);
	}
	else if (economy->demand.workers < economy->supply.food / 2 + economy->supply.water / 2) {
		economy->demand.workers++;
	}
}