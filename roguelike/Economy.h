#pragma once
#define TCOD_SDL2
#define MAX_GVAL 9999

#include <libtcod.hpp>

#include <vector>

#include <iostream>

class Spaceship;
class Faction;
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
		int consumer_goods = 0;
		int industrial_goods = 0;
		int luxury_goods = 0;
		int military_goods = 0;
		int minerals = 0;
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

		void print_vals() {
			std::cout << food << std::endl;
			std::cout << water << std::endl;
			std::cout << air << std::endl;
			std::cout << consumer_goods << std::endl;
			std::cout << industrial_goods << std::endl;
			std::cout << luxury_goods << std::endl;
			std::cout << military_goods << std::endl;
			std::cout << minerals << std::endl;
			std::cout << workers << std::endl;
		}

		void reset() {
			std::vector<int> v;
			for (int i = 0; i < get_vals().size(); i++) {
				v.push_back(0);
			}
			set_vals(v);
		}

		void serialize(TCODZip* zip) {
			zip->putInt(food);
			zip->putInt(water);
			zip->putInt(air);
			zip->putInt(consumer_goods);
			zip->putInt(industrial_goods);
			zip->putInt(luxury_goods);
			zip->putInt(military_goods);
			zip->putInt(minerals);
			zip->putInt(workers);
		}

		void deserialize(TCODZip* zip) {
			food = zip->getInt();
			water = zip->getInt();
			air = zip->getInt();
			consumer_goods = zip->getInt();
			industrial_goods = zip->getInt();
			luxury_goods = zip->getInt();
			military_goods = zip->getInt();
			minerals = zip->getInt();
			workers = zip->getInt();
		}

		friend bool operator== (Goods &a1, Goods &a2);
		friend bool operator!= (Goods &a1, Goods &a2);
		friend Goods operator+ (Goods &a1, Goods &a2);
		friend Goods operator- (Goods &a1, Goods &a2);
		friend Goods operator* (Goods &a1, Goods &a2);
		friend Goods operator/ (Goods &a1, Goods &a2);
		friend void operator+= (Goods &a1, Goods &a2);
		friend void operator-= (Goods &a1, Goods &a2);
		friend void operator*= (Goods &a1, Goods &a2);
		friend void operator/= (Goods &a1, Goods &a2);
		friend bool operator< (Goods &a1, Goods &a2);
		friend bool operator<= (Goods &a1, Goods &a2);
		friend bool operator> (Goods &a1, Goods &a2);
		friend bool operator>= (Goods &a1, Goods &a2);

#if 1
		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator== (Economy::Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			for (int i = 0; i < a1v.size(); i++) {
				if (a1v[i] != a2) {
					return false;
				}
			}
			return true;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator!= (Economy::Goods &a1, const T &a2) {
			return !(a1 == a2);
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator< (Economy::Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			int sum = 0;
			for (int i = 0; i < a1v.size(); i++) {
				sum += a1v[i];
			}
			return sum < a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator<= (Economy::Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			int sum = 0;
			for (int i = 0; i < a1v.size(); i++) {
				sum += a1v[i];
			}
			return sum <= a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator> (Economy::Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			int sum = 0;
			for (int i = 0; i < a1v.size(); i++) {
				sum += a1v[i];
			}
			return sum > a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend bool operator>= (Economy::Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			int sum = 0;
			for (int i = 0; i < a1v.size(); i++) {
				sum += a1v[i];
			}
			return sum >= a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend Goods operator+ (Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			for (int i = 0; i < a1v.size(); i++) {
				a1v[i] += a2;
			}
			Economy::Goods g;
			g.set_vals(a1v);
			return g;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend Goods operator- (Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			for (int i = 0; i < a1v.size(); i++) {
				a1v[i] -= a2;
			}
			Economy::Goods g;
			g.set_vals(a1v);
			return g;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend Goods operator* (Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			for (int i = 0; i < a1v.size(); i++) {
				a1v[i] *= a2;
			}
			Economy::Goods g;
			g.set_vals(a1v);
			return g;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend Goods operator/ (Goods &a1, const T &a2) {
			auto a1v = a1.get_vals();
			for (int i = 0; i < a1v.size(); i++) {
				a1v[i] /= a2;
			}
			Economy::Goods g;
			g.set_vals(a1v);
			return g;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend void operator+= (Goods &a1, const T &a2) {
			a1 = a1 + a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend void operator-= (Goods &a1, const T &a2) {
			a1 = a1 - a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend void operator*= (Goods &a1, const T &a2) {
			a1 = a1 * a2;
		}

		template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		friend void operator/= (Goods &a1, const T &a2) {
			a1 = a1 / a2;
		}
#endif
	} supply, demand;

	class Building {
	public:
		enum BUILDING_TYPES {
			FARMING_COMPLEX,
			MINING_COMPLEX,
			INDUSTRIAL_COMPLEX,
		};

		Building::Building() {}
		Building::Building(Economy* e);

		virtual void update() { std::cout << "DEBUG ERROR: A BUILDING HAS NO UPDATE FUNCTION\n"; }
		Economy* economy;

		Goods cost;
		unsigned int tier = 1;

		std::string name = "Unnamed";
		std::string initial = "B";

		TCODColor color = TCODColor::white;

		virtual void serialize(TCODZip* zip);
		virtual void deserialize(TCODZip* zip);
	};

	~Economy();

	void update();

	std::vector<int> supply_values();
	std::vector<int> demand_values();

	void goods_change(SUPPLY_TYPES type, std::vector<int>& v);

	void print_values();

	std::vector<Building*> buildings;

	std::vector<Building*> underconstruction_buildings;

	std::vector<Goods> supply_history;
	std::vector<Goods> demand_history;

	bool build_building(Building* b);
	bool build_ship(Spaceship* s, Faction* f);

	void construct_buildings();

	bool has_building(const std::string& type);

	Goods trend(SUPPLY_TYPES type);

	void serialize(TCODZip* zip);
	void deserialize(TCODZip* zip);
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
public:
	SpacePort(Economy* e);

	virtual void update() override;
};

class MIC : public Economy::Building {
public:
	MIC(Economy* e);

	virtual void update() override;
};

class Commercial : public Economy::Building {
public:
	Commercial(Economy* e);

	virtual void update() override;
};
}
