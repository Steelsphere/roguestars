#include "GUI.h"
#include "GameObjects.h"
#include "Input.h"

#include <algorithm>
#include <cstring>

std::vector<GUI*> GUI::_buffer;

GUI::GUI() {}

GUI::GUI(int x, int y, int w, int h, std::vector<Text> text) : 
_x(x), _y(y), _width(w), _height(h), _text(text)
{
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
	_buffer.push_back(this);
}

GUI::~GUI()
{
	_buffer.erase(std::remove(_buffer.begin(), _buffer.end(), this), _buffer.end());
	TCODConsole::root->rect(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(0.0f));
	GameObjects::update = true;
	std::cout << "GUIs: " << _buffer.size() << std::endl;
}

void GUI::draw() {
	TCODConsole::root->printFrame(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(1.0f));
	for (int i = 0; i < _text.size(); i++) {
		TCODConsole::setColorControl(TCOD_COLCTRL_1, _text[i].color, TCODColor::black);
		TCODConsole::root->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
	}
}

void GUI::position_text() {
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
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
	GUI::draw();
	if (Input::get_mode() == Input::NORMAL) {
		Message_Box::~Message_Box();
	}
}

Log::Log() : GUI(0, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>()) {
	Text name = { (_width/2) - 3, 0, 3, 1, "Log", TCODColor::red };
	_text.push_back(name);
	position_text();
}

void Log::message(std::string message, TCODColor color) {
	for (int i = 1; i < _text.size(); i++) {
		_text[i].y -= 1;
		if (_text[i].y <= _y) {
			_text.erase(_text.begin() + i);
		}
	}
	Text m = { 1, _height - 2, _width - 1, 1, message, color };
	m.x += _x;
	m.y += _y;
	_text.push_back(m);
}

Status::Status() : GUI(GameObjects::screen_width / 2, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>()) {
	Text name = { (_width / 2) - 3, 0, 6, 1, "Status", TCODColor::red };
	_text.push_back(name);
	position_text();
}

SelectionBox::SelectionBox() {}

SelectionBox::SelectionBox(int x, int y, int w, int h, std::vector<Text> text) : GUI(x, y, w, h, text) {}

void SelectionBox::draw() {
	GUI::draw();
	for (int i = 0; i < _mtext.size(); i++) {
		TCODConsole::setColorControl(TCOD_COLCTRL_1, _mtext[i].color, TCODColor::black);
		TCODConsole::root->printRect(_mtext[i].x, _mtext[i].y, _mtext[i].w, _mtext[i].h, (std::string("%c") + _mtext[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
	}
	if (Input::get_last_key().vk == TCODK_DOWN && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				_mtext[i].selected = false;
				if (i == _mtext.size() - 1) {
					_mtext[0].selected = true;
				}
				else {
					_mtext[i + 1].selected = true;
				}
				set_selector();
				break;
			}
		}
	}
	if (Input::get_last_key().vk == TCODK_UP && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				_mtext[i].selected = false;
				if (i == 0) {
					_mtext[_mtext.size() - 1].selected = true;
				}
				else {
					_mtext[i - 1].selected = true;
				}
				set_selector();
				break;
			}
		}
	}
	if (Input::get_last_key().vk == TCODK_ENTER && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				GameEvent::set_event(_mtext[i].action);
			}
		}
	}
}

void SelectionBox::position_text() {
	GUI::position_text();
	for (int i = 0; i < _mtext.size(); i++) {
		_mtext[i].x += _x;
		_mtext[i].y += _y;
	}
}

void SelectionBox::set_selector() {
	for (MText& i : _mtext) {
		if (i.selected) {
			i.color = TCODColor::yellow;
		}
		else {
			i.color = TCODColor::white;
		}
	}
}

MainMenu::MainMenu() : SelectionBox(0, 0, GameObjects::screen_width, GameObjects::screen_height, std::vector<Text>()) {
	front();
}

void MainMenu::front() {
	_text.clear();
	_mtext.clear();

	Text title = { (GameObjects::screen_width / 2) - 5, 10, 20, 1, "Roguelike!", TCODColor::red };
	_text.push_back(title);
	
	MText n1 = { (GameObjects::screen_width / 2) - 5, 20, 20, 1, "New Game", TCODColor::white, 1, GameEvent::STARTUP_NEW_GAME };
	MText n2 = { (GameObjects::screen_width / 2) - 5, 22, 20, 1, "Load Game", TCODColor::white, 0, GameEvent::STARTUP_LOAD_GAME };
	MText n3 = { (GameObjects::screen_width / 2) - 5, 24, 20, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };
	
	_mtext.push_back(n1); _mtext.push_back(n2); _mtext.push_back(n3);
	
	position_text();
	set_selector();
	Input::set_mode(Input::NONE);
}

ESCMenu::ESCMenu() : SelectionBox(GameObjects::screen_width / 2 - 13, GameObjects::screen_height / 2 - 10, 24, 12, std::vector<Text>()) {
	MText n1 = { (_width / 2) - 5, 2, _width / 2, 1, "Main Menu", TCODColor::white, 1, GameEvent::TO_MAIN_MENU };
	MText n2 = { (_width / 2) - 5, 3, _width / 2, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };

	_mtext.push_back(n1); _mtext.push_back(n2);
	
	position_text();
	set_selector();
	Input::set_mode(Input::ESC);
}
