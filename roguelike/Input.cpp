#include "Input.h"

int Input::_cooldown = 100;
int Input::_cdfirst = TCODSystem::getElapsedMilli();
Actor* Input::_reciever;

void Input::input(TCOD_key_t key) {
	if (key.pressed) {
		switch (key.vk) {
		case TCODK_KP8:
			_reciever->move("top");
			break;
		case TCODK_KP7:
			_reciever->move("topleft");
			break;
		case TCODK_KP9:
			_reciever->move("topright");
			break;
		case TCODK_KP4:
			_reciever->move("left");
			break;
		case TCODK_KP6:
			_reciever->move("right");
			break;
		case TCODK_KP1:
			_reciever->move("bottomleft");
			break;
		case TCODK_KP2:
			_reciever->move("bottom");
			break;
		case TCODK_KP3:
			_reciever->move("bottomright");
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