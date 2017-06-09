#include "Light.h"
#include "Actor.h"


Light::LightSystem::LightSystem() {

}

Light::LightSystem::~LightSystem() {

}

void Light::LightSystem::set_global_lighting(Level* l, int r, int g, int b) {
	for (Actor* i : (*l->get_actors())) {
		i->set_bcolor_obj(TCODColor(r, g, b));
	}
}