#include "Spaceship.h"
#include "Tile.h"
#include "Faction.h"
#include "Economy.h"


Spaceship::Spaceship(int x, int y, char c, StarSector* s, Faction* f) : Actor(x, y, 0, c, TCODColor::white, TCODColor::black, "Ship")  {
	if (!s->economy.has_building("Space Port")) {
		delete this;
		std::cout << "Failed to build ship in " << s->alias << ". Reason: no space port\n";
		return;
	}
	auto a = s->economy.supply.get_vals();
	auto b = cost.get_vals();
	for (int i = 0; i < a.size(); i++) {
		if (a[i] < b[i]) {
			delete this;
			std::cout << "Failed to build ship in " << s->alias << ". Reason: insufficient materials\n";
			return;
		}
	}
}


Spaceship::~Spaceship() {

}
