#pragma once

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
	};
	
	~Economy();

	void update();

	std::vector<int> supply_values();
	std::vector<int> demand_values();

	void goods_change(SUPPLY_TYPES type, std::vector<int>& v);

	void print_values();

	std::vector<Building*> buildings;

	std::vector<Building::BUILDING_TYPES> wanted_buildings;
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
}
