#include "Light.h"
#include "Actor.h"


LightSystem::LightSystem() {

}

LightSystem::~LightSystem() {

}

void LightSystem::set_global_lighting(Level* l, int r, int g, int b) {
	for (Actor* i : (*l->get_actors())) {
		set_light(i, TCODColor(r, g, b));
	}
}

void LightSystem::set_light(Actor* a, TCODColor l) {
	_original_colors[a] = a->get_bcolor_obj();
	a->set_bcolor_obj(l);
//	std::cout << "Set light at: " << &a << " to " << l.getHue() << " " << l.getSaturation() << " " << l.getValue() << std::endl;
}

void LightSystem::remove_light(Actor* a) {
	a->set_bcolor_obj(_original_colors[a]);
//	std::cout << "Removed light at: " << &a << std::endl;
}

void LightSystem::remove_all_lights(Level* l) {
	for (Actor* i : (*l->get_actors())) {
		remove_light(i);
	}
}