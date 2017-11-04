#pragma once

#include <vector>

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
		Building::Building(Economy* e);

		virtual void update() {}
		Economy* economy;
	};
	

	void update();

	std::vector<int> supply_values();
	std::vector<int> demand_values();

	void goods_change(SUPPLY_TYPES type, std::vector<int>& v);

	void print_values();

	std::vector<Building> buildings;
};

namespace Buildings {
	class FarmingComplex : public Economy::Building {
		virtual void update() override;
	};

	class MiningComplex : public Economy::Building {
		virtual void update() override;
	};
}
