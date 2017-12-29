#include "Camera.h"
#include "GameObjects.h"


Camera::Camera(Actor* following)
{
	_following = following;
	int* sxyz = _following->get_screen_pos();
	int* wxyz = _following->get_world_pos();
	_screen_x = sxyz[0];
	_screen_y = sxyz[1];
	_screen_z = sxyz[2];
	_world_x = wxyz[0];
	_world_y = wxyz[1];
	_world_z = wxyz[2];
}

Camera::Camera(int x, int y) {
	_following = nullptr;
	_screen_x = x;
	_screen_y = y;
	_screen_z = 0;
	_world_x = x;
	_world_y = y;
	_world_z = 0;
}

Camera::~Camera()
{
}

void Camera::update() {
	if (_following == nullptr) {
		return;
	}
	
	int* sxyz = _following->get_screen_pos();
	int* wxyz = _following->get_world_pos();
	_screen_x = sxyz[0];
	_screen_y = sxyz[1];
	_screen_z = sxyz[2];
	_world_x = wxyz[0];
	_world_y = wxyz[1];
	_world_z = wxyz[2];

	std::vector<Actor*>* actors = _level->get_actors();
	Actor* a;
	int* ar;

	for (int i = 0; i < actors->size(); i++) {
		a = actors->operator[](i);
		ar = a->get_screen_pos();
		a->set_position(ar[0] - _screen_x + GameObjects::screen_width / 2,
		ar[1] - _screen_y + GameObjects::screen_height / 2, 0);
	}
}

int* Camera::get_screen_pos() {
	static int a[3];
	a[0] = _screen_x;
	a[1] = _screen_y;
	a[2] = _screen_z;
	return a;
}

int* Camera::get_world_pos() {
	static int a[3];
	a[0] = _world_x;
	a[1] = _world_y;
	a[2] = _world_z;
	return a;
}