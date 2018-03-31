#pragma once
#define TCOD_SDL2
#include "Level.h"

#include <map>

class Light {
public:

	class LightSystem {
	public:
		LightSystem();
		~LightSystem();

		void set_global_lighting(Level* l, int r, int b, int g);

		void set_light(Actor* a, TCODColor l);

		void remove_light(Actor* a);

		void remove_all_lights(Level* l);

		void clear_cache() { _original_colors.clear(); }
	private:
		std::map<Actor*, TCODColor> _original_colors;
	};
};
