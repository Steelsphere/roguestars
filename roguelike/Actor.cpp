#include "Actor.h"
#include "GameObjects.h"

std::vector<Actor*>* Actor::_buffer;

Actor::Actor()
{
	_buffer->push_back(this);
}

Actor::Actor(int x, int y, int z, std::string name) :
	_screen_x(x), _screen_y(y), _screen_z(z), _world_x(x), _world_y(y), _world_z(z), _name(name), _impassable(false), _transparent(true)
{
	_buffer->push_back(this);
}

Actor::Actor(int x, int y, int z, char c, TCODColor fcolor, TCODColor bcolor, std::string name) : 
	_screen_x(x), _screen_y(y), _screen_z(z), _c(c), _fcolor(fcolor), _bcolor(bcolor), _world_x(x), _world_y(y), _world_z(z), _name(name), _impassable(false), _transparent(true)
{
	_buffer->push_back(this);
}


Actor::~Actor()
{
	_buffer->erase(std::remove(_buffer->begin(), _buffer->end(), this), _buffer->end());
}

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
	TCODConsole::root->putCharEx(_screen_x, _screen_y, 0, TCODColor::black, TCODColor::black);
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
	TCODConsole::root->putChar(_screen_x, _screen_y, _c);
	TCODConsole::root->setCharForeground(_screen_x, _screen_y, _fcolor);
	TCODConsole::root->setCharBackground(_screen_x, _screen_y, _bcolor);
}

Actor* Actor::get_actor(int x, int y, int z) {
//	std::cout << "Get actor called at: " << x << " " << y << " " << std::endl;
//	std::cout << "Player pos: " << GameObjects::player->get_screen_pos()[0] << " " << GameObjects::player->get_screen_pos()[1] << std::endl;
	for (int i = 0; i < _buffer->size(); i++) {
		if (_buffer->operator[](i)->get_screen_pos()[0] == x && _buffer->operator[](i)->get_screen_pos()[1] == y && _buffer->operator[](i)->get_screen_pos()[2] == z) {
			return _buffer->at(i);
		}
	}
	return nullptr;
}

std::vector<Actor*> Actor::get_actors(int x, int y, int z) {
	std::vector<Actor*> a;
	for (int i = 0; i < _buffer->size(); i++) {
		if (_buffer->operator[](i)->get_screen_pos()[0] == x && _buffer->operator[](i)->get_screen_pos()[1] == y && _buffer->operator[](i)->get_screen_pos()[2] == z) {
			a.push_back(_buffer->operator[](i));
		}
	}
	return a;
}

std::map<std::string, Actor*> Actor::get_adjacent_actors() {
	std::map<std::string, Actor*> m;
	m["topleft"] = get_actor(_screen_x - 1, _screen_y - 1, _screen_z);
	m["top"] = get_actor(_screen_x, _screen_y - 1, _screen_z);
	m["topright"] = get_actor(_screen_x + 1, _screen_y - 1, _screen_z);
	m["left"] = get_actor(_screen_x - 1, _screen_y, _screen_z);
	m["right"] = get_actor(_screen_x + 1, _screen_y, _screen_z);
	m["bottomleft"] = get_actor(_screen_x - 1, _screen_y + 1, _screen_z);
	m["bottom"] = get_actor(_screen_x, _screen_y + 1, _screen_z);
	m["bottomright"] = get_actor(_screen_x + 1, _screen_y + 1, _screen_z);
	return m;
}

void Actor::move(std::string dir) {
	int xm = GameObjects::map_dir.at(dir).first;
	int ym = GameObjects::map_dir.at(dir).second;

	std::vector<Actor*> checkvec = Actor::get_actors(_screen_x + xm, _screen_y + ym, _screen_z);
	for (int i = 0; i < checkvec.size(); i++) {
		if (checkvec[i]->is_impassable()) {
			return;
		}
	}

	this->set_position(_screen_x + xm, _screen_y + ym, _screen_z);
	this->set_world_position(_world_x + xm, _world_y + ym, _world_z);
}

void Actor::set_buffer(std::vector<Actor*>* buffer) {
	_buffer = buffer;
}

