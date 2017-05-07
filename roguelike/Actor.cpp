#include "Actor.h"
#include "GameObjects.h"

std::vector<Actor*>* Actor::_buffer;
std::vector<std::vector<std::vector<Actor*>>>* Actor::_map;

Actor::Actor()
{
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
	if (std::find(_buffer->begin(), _buffer->end(), this) != _buffer->end()) {
		_buffer->erase(std::remove(_buffer->begin(), _buffer->end(), this), _buffer->end());
	}
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
	return _map->operator[](x)[y][0];
}

std::vector<Actor*> Actor::get_actors(int x, int y, int z) {
	std::vector<Actor*> a;
	
	if (x < 0 || y < 0 || x >= _map->size() || y >= _map->size()) {
		return a;
	}

	for (int i = 0; i < _map->operator[](x)[y].size(); i++) {
			a.push_back(_map->operator[](x)[y][i]);
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

void Actor::get_color(float* h, float* s, float* v) {
	_fcolor.getHSV(h, s, v);
}

void Actor::serialize(std::ofstream* os) {
	os->write(reinterpret_cast<char*>(&_screen_x), sizeof(_screen_x));
	os->write(reinterpret_cast<char*>(&_screen_y), sizeof(_screen_y));
	os->write(reinterpret_cast<char*>(&_screen_z), sizeof(_screen_z));
	
	os->write(reinterpret_cast<char*>(&_world_x), sizeof(_world_x));
	os->write(reinterpret_cast<char*>(&_world_y), sizeof(_world_y));
	os->write(reinterpret_cast<char*>(&_world_z), sizeof(_world_z));
	
	os->write(reinterpret_cast<char*>(&_c), sizeof(_c));

	float fh, fs, fv;
	_fcolor.getHSV(&fh, &fs, &fv);

	os->write(reinterpret_cast<char*>(&fh), sizeof(fh));
	os->write(reinterpret_cast<char*>(&fs), sizeof(fs));
	os->write(reinterpret_cast<char*>(&fv), sizeof(fv));

	float bh, bs, bv;
	_bcolor.getHSV(&bh, &bs, &bv);

	os->write(reinterpret_cast<char*>(&bh), sizeof(bh));
	os->write(reinterpret_cast<char*>(&bs), sizeof(bs));
	os->write(reinterpret_cast<char*>(&bv), sizeof(bv));

	os->write(reinterpret_cast<char*>(&_name), sizeof(_name));
	os->write(reinterpret_cast<char*>(&_impassable), sizeof(_impassable));
	os->write(reinterpret_cast<char*>(&_transparent), sizeof(_transparent));

}

void Actor::deserialize(std::ifstream* is, Actor* actor) {
	is->read(reinterpret_cast<char*>(&actor->_screen_x), sizeof(_screen_x));
	is->read(reinterpret_cast<char*>(&actor->_screen_y), sizeof(_screen_y));
	is->read(reinterpret_cast<char*>(&actor->_screen_z), sizeof(_screen_z));

	is->read(reinterpret_cast<char*>(&actor->_world_x), sizeof(_world_x));
	is->read(reinterpret_cast<char*>(&actor->_world_y), sizeof(_world_y));
	is->read(reinterpret_cast<char*>(&actor->_world_z), sizeof(_world_z));

	is->read(reinterpret_cast<char*>(&actor->_c), sizeof(_c));

	float fh, fs, fv;
	
	is->read(reinterpret_cast<char*>(&fh), sizeof(fh));
	is->read(reinterpret_cast<char*>(&fs), sizeof(fs));
	is->read(reinterpret_cast<char*>(&fv), sizeof(fv));

	actor->_fcolor = TCODColor(fh, fs, fv);

	float bh, bs, bv;
	
	is->read(reinterpret_cast<char*>(&bh), sizeof(bh));
	is->read(reinterpret_cast<char*>(&bs), sizeof(bs));
	is->read(reinterpret_cast<char*>(&bv), sizeof(bv));

	actor->_bcolor = TCODColor(bh, bs, bv);

	is->read(reinterpret_cast<char*>(&actor->_name), sizeof(_name));
	is->read(reinterpret_cast<char*>(&actor->_impassable), sizeof(_impassable));
	is->read(reinterpret_cast<char*>(&actor->_transparent), sizeof(_transparent));
}