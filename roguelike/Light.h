#pragma once
#include "Level.h"

class Light {
public:
	
	class LightSystem {
	public:
		LightSystem();
		~LightSystem();

		void set_global_lighting(Level* l, int r, int b, int g);
	};
};

