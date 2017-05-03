#include "GUI.h"
#include <algorithm>
#include "GameObjects.h"
#include <cstring>
#include "Input.h"

std::vector<GUI*>* GUI::_buffer;

GUI::GUI(int x, int y, int w, int h, std::vector<Text> text) : 
_x(x), _y(y), _width(w), _height(h), _text(text)
{
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
	_buffer->push_back(this);
}

GUI::GUI(GUI_TYPE type, std::string text) {
	_buffer->push_back(this);

	switch (type) {
	case MESSAGE_BOX:
		_x = GameObjects::screen_width/2 - 13;
		_y = GameObjects::screen_height / 2 - 10;
		_width = 24;
		_height = 12;
		Text top = { 6, 0, 12, 2, "Announcement", TCODColor::red };
		Text message = { 6, 4, 12, 12, text, TCODColor::white };
		Text bottom = { 6, 10, 12, 2, "Press enter to continue", TCODColor::red };
		_text.push_back(top);
		_text.push_back(message);
		_text.push_back(bottom);
		break;
	}
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
}

GUI::~GUI()
{
	_buffer->erase(std::remove(_buffer->begin(), _buffer->end(), this), _buffer->end());
	TCODConsole::root->rect(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(0.0f));
	GameObjects::update = true;
}

void GUI::draw() {
	TCODConsole::root->printFrame(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(1.0f));
	for (int i = 0; i < _text.size(); i++) {
		TCODConsole::setColorControl(TCOD_COLCTRL_1, _text[i].color, TCODColor::black);
		TCODConsole::root->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
	}
}

Message_Box::Message_Box(std::string text) : GUI(GameObjects::screen_width/2 - 13, GameObjects::screen_height/2 - 10, 24, 12, std::vector<Text>()) {
	Text top = { 6, 0, 12, 2, "Announcement", TCODColor::red };
	Text message = { 6, 4, 12, 12, text, TCODColor::white };
	Text bottom = { 6, 10, 12, 2, "Press enter to continue", TCODColor::red };
	_text.push_back(top);
	_text.push_back(message);
	_text.push_back(bottom);
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
	Input::set_mode(Input::ENTER_TO_CONTINUE);
}

void Message_Box::draw() {
	TCODConsole::root->printFrame(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(1.0f));
	for (int i = 0; i < _text.size(); i++) {
		TCODConsole::setColorControl(TCOD_COLCTRL_1, _text[i].color, TCODColor::black);
		TCODConsole::root->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
	}
	if (Input::get_mode() == Input::NORMAL) {
		Message_Box::~Message_Box();
	}
}