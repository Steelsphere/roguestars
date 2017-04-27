#include "Input.h"

int Input::_cooldown = 100;
int Input::_cdfirst = TCODSystem::getElapsedMilli();

void Input::input(TCOD_key_t key) {
	if (key.pressed) {
		switch (key.vk) {
		case TCODK_KP8:
			GameObjects::player->move("top");
			break;
		case TCODK_KP7:
			GameObjects::player->move("topleft");
			break;
		case TCODK_KP9:
			GameObjects::player->move("topright");
			break;
		case TCODK_KP4:
			GameObjects::player->move("left");
			break;
		case TCODK_KP6:
			GameObjects::player->move("right");
			break;
		case TCODK_KP1:
			GameObjects::player->move("bottomleft");
			break;
		case TCODK_KP2:
			GameObjects::player->move("bottom");
			break;
		case TCODK_KP3:
			GameObjects::player->move("bottomright");
			break;
		}
	}
}

bool Input::calc_key_cooldown() {
	std::cout << TCODSystem::getElapsedMilli() << std::endl;
	std::cout << _cdfirst << std::endl;
	if (TCODSystem::getElapsedMilli() - _cooldown > _cdfirst) {
		_cdfirst = TCODSystem::getElapsedMilli();
		return true;
	}
	return false;
}