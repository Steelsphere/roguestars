#pragma once
#define TCOD_SDL2

#include <libtcod.hpp>

#include <vector>

#include <iostream>

class Economy {
public:
	enum SUPPLY_TYPES {
		SUPPLY,
		DEMAND
	};
	
	struct Goods {
		int food = 0;
		int water = 0;
		int air = 0;
		int minerals = 0;
		int industrial_goods = 0;
		int luxury_goods = 0;
		int consumer_goods = 0;
		int military_goods = 0;
		int workers = 0;

		std::vector<int> get_vals() {
			std::vector<int> v;
			v.push_back(food);
			v.push_back(water);
			v.push_back(air);
			v.push_back(consumer_goods);
			v.push_back(industrial_goods);
			v.push_back(luxury_goods);
			v.push_back(military_goods);
			v.push_back(minerals);
			v.push_back(workers);
			return v;
		}

		void set_vals(std::vector<int>& v) {
			food = v[0];
			water = v[1];
			air = v[2];
			consumer_goods = v[3];
			industrial_goods = v[4];
			luxury_goods = v[5];
			military_goods = v[6];
			minerals = v[7];
			workers = v[8];
		}
	} supply, demand;

	class Building {
	public:
		enum BUILDING_TYPES {
			FARMING_COMPLEX,
			MINING_COMPLEX,
			INDUSTRIAL_COMPLEX,
		};
		
		Building::Building(Economy* e);

		virtual void update() { std::cout << "DEBUG ERROR: A BUILDING HAS NO UPDATE FUNCTION\n"; }
		Economy* economy;

		Goods cost;
		unsigned int tier = 1;

		std::string name = "Unnamed";
		std::string initial = "B";

		TCODColor color = TCODColor::white;
	};
	
	~Economy();

	void update();

	std::vector<int> supply_values();
	std::vector<int> demand_values();

	void goods_change(SUPPLY_TYPES type, std::vector<int>& v);

	void print_values();

	std::vector<Building*> buildings;

	std::vector<Building*> underconstruction_buildings;

	bool build_building(Building* b);

	void construct_buildings();

	bool has_building(const std::string& type);
};

namespace Buildings {
	class FarmingComplex : public Economy::Building {
	public:
		FarmingComplex(Economy* e);
		
		virtual void update() override;
	};

	class MiningComplex : public Economy::Building {
	public:
		MiningComplex(Economy* e);
		
		virtual void update() override;
	};

	class IndustrialComplex : public Economy::Building {
	public:
		IndustrialComplex(Economy* e);

		virtual void update() override;
	};

	class Infrastructure : public Economy::Building {
	public:
		Infrastructure(Economy* e);

		virtual void update() override;
	};

	class SpacePort : public Economy::Building {
		SpacePort(Economy* e);
	};
}
