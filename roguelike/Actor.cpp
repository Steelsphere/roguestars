#include "Actor.h"
#include "GameObjects.h"
#include "Item.h"
#include "Input.h"
#include "Camera.h"

#include <typeinfo>
#include <iostream>

std::vector<Actor*>* Actor::_buffer;
std::vector<std::vector<std::vector<Actor*>>>* Actor::_map = nullptr;

Actor::Actor()
{
}

Actor::Actor(int x, int y, int z, const std::string& name) :
	_screen_x(x - GameObjects::camera->get_world_pos()[0]),
	_screen_y(y - GameObjects::camera->get_world_pos()[1]),
	_screen_z(z), 
	_world_x(x), 
	_world_y(y), 
	_world_z(z), 
	_name(name), 
	_impassable(false), 
	_transparent(true), 
	_memorized(false),
	_in_fov(false)
{
	_buffer->push_back(this);
	if (_map != nullptr) {
		(*_map)[x][y].push_back(this);
	}
}

Actor::Actor(int x, int y, int z, char c, TCODColor fcolor, TCODColor bcolor, const std::string& name) : 
	_screen_x(x - GameObjects::camera->get_world_pos()[0] + GameObjects::screen_width / 2), 
	_screen_y(y - GameObjects::camera->get_world_pos()[1] + GameObjects::screen_height / 2),
	_screen_z(z), 
	_c(c), 
	_fcolor(fcolor), 
	_bcolor(bcolor), 
	_world_x(x), 
	_world_y(y), 
	_world_z(z), 
	_name(name), 
	_impassable(false), 
	_transparent(true),
	_memorized(false),
	_in_fov(false)
{
	_buffer->push_back(this);
	if (_map != nullptr) {
		_map->operator[](x)[y].push_back(this);
	}
}


Actor::~Actor() {}

int* Actor::get_screen_pos() {
	static int a[3];
	a[0] = _screen_x;
	a[1] = _screen_y;
	a[2] = _screen_z;
	return a;
}

int* Actor::get_world_pos() {
	static int a[3];
	a[0] = _world_x;
	a[1] = _world_y;
	a[2] = _world_z;
	return a;
}

void Actor::set_position(int x, int y, int z) {
	if (_screen_x < TCODConsole::root->getWidth() && _screen_y < TCODConsole::root->getHeight() &&
		_screen_x > 0 && _screen_y > 0) {
		TCODConsole::root->putCharEx(_screen_x, _screen_y, 0, TCODColor::black, TCODColor::black);
	}
	_screen_x = x;
	_screen_y = y;
	_screen_z = z;
}

void Actor::set_world_position(int x, int y, int z) {
	_world_x = x;
	_world_y = y;
	_world_z = z;
}

void Actor::draw() {
	if (!(_screen_x < TCODConsole::root->getWidth() && _screen_y < TCODConsole::root->getHeight() &&
		_screen_x >= 0 && _screen_y >= 0)) {
		return;
	}
	TCODConsole::root->putChar(_screen_x, _screen_y, _c);
	TCODConsole::root->setCharForeground(_screen_x, _screen_y, _fcolor);
	if (Input::get_last_mouse().cx == _screen_x && Input::get_last_mouse().cy == _screen_y) {
		TCODConsole::root->setCharBackground(_screen_x, _screen_y, TCODColor::darkGreen);
	}
	else {
		TCODConsole::root->setCharBackground(_screen_x, _screen_y, _bcolor);
	}
}

void Actor::draw_mem() {
	if (!(_screen_x < TCODConsole::root->getWidth() && _screen_y < TCODConsole::root->getHeight() &&
		_screen_x > 0 && _screen_y > 0)) {
		return;
	}
	TCODConsole::root->putChar(_screen_x, _screen_y, _c);
	TCODConsole::root->setCharForeground(_screen_x, _screen_y, TCODColor::darkCyan);
	TCODConsole::root->setCharBackground(_screen_x, _screen_y, _bcolor);
}

Actor* Actor::get_actor(int x, int y, int z) {
	if (x >= _map->size() || x < 0) {
		return nullptr;
	}
	if (y >= _map[0].size() || y < 0) {
		return nullptr;
	}
	return (*_map)[x][y].back();
}

std::vector<Actor*> Actor::get_actors(int x, int y, int z) {
	std::vector<Actor*> a;
	
	if (x < 0 || y < 0 || x >= _map->size() || y >= _map->size()) {
		return a;
	}

	for (int i = 0; i < (*_map)[x][y].size(); i++) {
		a.push_back((*_map)[x][y][i]);
	}
	
	return a;
}

std::map<std::string, Actor*> Actor::get_adjacent_actors() {
	std::map<std::string, Actor*> m;
	m["topleft"] = get_actor(_world_x - 1, _world_y - 1, _world_z);
	m["top"] = get_actor(_world_x, _world_y - 1, _world_z);
	m["topright"] = get_actor(_world_x + 1, _world_y - 1, _world_z);
	m["left"] = get_actor(_world_x - 1, _world_y, _world_z);
	m["right"] = get_actor(_world_x + 1, _world_y, _world_z);
	m["bottomleft"] = get_actor(_world_x - 1, _world_y + 1, _world_z);
	m["bottom"] = get_actor(_world_x, _world_y + 1, _world_z);
	m["bottomright"] = get_actor(_world_x + 1, _world_y + 1, _world_z);
	return m;
}

std::vector<Actor*> Actor::get_adjacent_actors_vec() {
	std::vector<Actor*> v;
	v.resize(8);
	v[0] = get_actor(_world_x - 1, _world_y - 1, _world_z);
	v[1] = get_actor(_world_x, _world_y - 1, _world_z);
	v[2] = get_actor(_world_x + 1, _world_y - 1, _world_z);
	v[3] = get_actor(_world_x - 1, _world_y, _world_z);
	v[4] = get_actor(_world_x + 1, _world_y, _world_z);
	v[5] = get_actor(_world_x - 1, _world_y + 1, _world_z);
	v[6] = get_actor(_world_x, _world_y + 1, _world_z);
	v[7] = get_actor(_world_x + 1, _world_y + 1, _world_z);
	return v;
}

void Actor::move(const std::string& dir) {
	int xm = GameObjects::map_dir.at(dir).first;
	int ym = GameObjects::map_dir.at(dir).second;

	std::vector<Actor*> checkvec = Actor::get_actors(_screen_x + xm, _screen_y + ym, _screen_z);
	std::vector<std::vector<std::vector<Actor*>>>* invec = Actor::get_map();

	if (checkvec.size() == 0) {
		return;
	}

	for (int i = 0; i < checkvec.size(); i++) {
		if (checkvec[i]->is_impassable()) {
//			checkvec[i]->on_collide();
			return;
		}
	}

	for (int i = 0; i < (*invec)[_world_x][_world_y].size(); i++) {
		if ((*invec)[_world_x][_world_y][i] == this) {
			(*invec)[_world_x][_world_y].erase((*invec)[_world_x][_world_y].begin() + i);
		}
	}

	(*invec)[_world_x + xm][_world_y + ym].push_back(this);
	
	for (Actor* a : Actor::get_actors(_world_x, _world_y, 0)) {
		Level::get_fov_map()->setProperties(_world_x, _world_y, a->is_transparent(), !a->is_impassable());
	}
	Level::get_fov_map()->setProperties(_world_x + xm, _world_y + ym, _transparent, !_impassable);
	
	this->set_position(_screen_x + xm, _screen_y + ym, _screen_z);
	this->set_world_position(_world_x + xm, _world_y + ym, _world_z);
}

void Actor::get_color(float* h, float* s, float* v) {
	_fcolor.getHSV(h, s, v);
}

void Actor::serialize(TCODZip* zip) {
	std::string typestring = typeid((*this)).name();
	
	if (typestring.size() == 0) {
		std::cerr << "Serialization error!\n";
	}
	
	zip->putString(typestring.c_str());
	zip->putInt(_screen_x); zip->putInt(_screen_y); zip->putInt(_screen_z);
	zip->putInt(_world_x); zip->putInt(_world_y); zip->putInt(_world_z);
	zip->putChar(_c);
	zip->putColor(&_fcolor); zip->putColor(&_bcolor);
	zip->putString(_name.c_str());
	zip->putInt(_impassable); zip->putInt(_transparent);
	zip->putInt(_in_fov);
}

void Actor::deserialize(TCODZip* zip) {
	_screen_x = zip->getInt(); _screen_y = zip->getInt(); _screen_z = zip->getInt();
	_world_x = zip->getInt(); _world_y = zip->getInt(); _world_z = zip->getInt();
	_c = zip->getChar();
	_fcolor = zip->getColor(); _bcolor = zip->getColor();
	_name = zip->getString();
	_impassable = zip->getInt(); _transparent = zip->getInt();
	_in_fov = zip->getInt();
}

bool operator== (const Actor &a1, const Actor &a2) {
	return (a1._c == a2._c && 
		a1._fcolor == a2._fcolor && 
		a1._bcolor == a2._bcolor && 
		a1._name == a2._name && 
		a1._impassable == a2._impassable && 
		a1._transparent == a2._transparent);
}

bool operator!= (const Actor &a1, const Actor &a2) {
	return (a1._c != a2._c &&
		a1._fcolor != a2._fcolor &&
		a1._bcolor != a2._bcolor &&
		a1._name != a2._name &&
		a1._impassable != a2._impassable &&
		a1._transparent != a2._transparent);
}

void Actor::set_transparent_background() {
	auto vec = get_actors(_world_x, _world_y, 0);
	int idx = std::find(vec.begin(), vec.end(), this) - vec.begin();
	if (idx == 0) {
		return;
	}
	_bcolor = vec[idx - 1]->get_bcolor_obj();
}

Actor* Actor::get_actor_scr(int x, int y) {
	for (Actor* a : (*_buffer)) {
		if (a->_screen_x == x && a->_screen_y == y) {
			return a;
		}
	}
	return nullptr;
}

void Actor::delete_actor() {
	_buffer->erase(std::remove(_buffer->begin(), _buffer->end(), this));
	auto* m = Actor::get_map();
	(*m)[_world_x][_world_y].erase(std::remove((*m)[_world_x][_world_y].begin(), (*m)[_world_x][_world_y].end(), this));
	std::cout << "Deleted actor at " << this << std::endl;
}

Vec2 Actor::get_screen_position() {
	return Vec2(_screen_x, _screen_y);
}

Vec2 Actor::get_world_position() {
	return Vec2(_world_x, _world_y);
}
