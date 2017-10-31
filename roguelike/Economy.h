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
	} supply, demand;

	class Building {
		enum BUILDING_TYPE {
			CONSUMER_FACTORY,
			MILITARY_FACTORY,
			MINE,
			FARM,
		};

		void update() {}
	};

	void update();

	std::vector<int> supply_values();
	std::vector<int> demand_values();

	void goods_change(SUPPLY_TYPES type, std::vector<int>& v);

	std::vector<Building> buildings;
};
