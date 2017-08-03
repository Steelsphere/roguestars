#include "GUI.h"
#include "GameObjects.h"
#include "Input.h"
#include "Actor.h"
#include "Player.h"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <iostream>
#include <filesystem>
#include <tuple>

std::vector<GUI*> GUI::_buffer;

GUI::GUI() {}

GUI::GUI(int x, int y, int w, int h, std::vector<Text> text) : 
_x(x), _y(y), _width(w), _height(h), _text(text), _transparency(1.0f), _update(true)
{
	_cons = new TCODConsole(w, h);
	_buffer.push_back(this);
}

GUI::~GUI()
{
	delete _cons;
	delete _screencons;
	
	_buffer.erase(std::remove(_buffer.begin(), _buffer.end(), this), _buffer.end());
	
	TCODConsole::root->rect(_x, _y, _width, _height, true, TCOD_BKGND_ALPHA(0.0f));
	GameObjects::update = true;
	std::cout << "GUIs: " << _buffer.size() << std::endl;
}

void GUI::draw(bool force) {
	if (_update || force) {
		switch (_type) {
		case FILLED_BACKGROUND:
			_cons->rect(0, 0, _width, _height, true);
			break;
		case FILLED_BORDERED_BACKGROUND:
			_cons->printFrame(0, 0, _width, _height, true);
			break;
		}
		for (int i = 0; i < _text.size(); i++) {
			_cons->setColorControl(TCOD_COLCTRL_1, _text[i].color, TCODColor::black);
			_cons->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
		}
		TCODConsole::blit(_cons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, _transparency);
		_update = false;
	}
}

void GUI::position_text() {
	for (int i = 0; i < _text.size(); i++) {
		_text[i].x += _x;
		_text[i].y += _y;
	}
}

void GUI::make_transparency_work() {
	if (_screencons == nullptr) {
		std::vector<std::vector<std::vector<std::tuple<char, TCODColor, TCODColor>>>> screen;
		screen.resize(_width);
		for (int y = 0; y < _width; y++) {
			screen[y].resize(_height);
		}

		for (int x = _x; x < _width; x++) {
			for (int y = _y; y < _height; y++) {
				screen[x][y].push_back(std::make_tuple(TCODConsole::root->getChar(x, y),
					TCODConsole::root->getCharForeground(x, y),
					TCODConsole::root->getCharBackground(x, y)));
			}
		}
		_screencons = new TCODConsole(_width, _height);
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				_screencons->putCharEx(x, y, std::get<0>(screen[x][y][0]), std::get<1>(screen[x][y][0]), std::get<2>(screen[x][y][0]));
			}
		}
		TCODConsole::blit(_screencons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, 1.0f);
	}
	else {
		TCODConsole::blit(_screencons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, 1.0f);
	}
}

Message_Box::Message_Box(std::string text, bool nocontinue) : GUI(GameObjects::screen_width/2 - 13, GameObjects::screen_height/2 - 10, 24, 12, std::vector<Text>()) {
	Text top = { 6, 0, 12, 2, "Announcement", TCODColor::red };
	Text message = { 6, 4, 12, 12, text, TCODColor::white };
	if (!nocontinue) {
		Text bottom = { 6, 10, 12, 2, "Press enter to continue", TCODColor::red };
		_text.push_back(bottom);
	}
	_text.push_back(top);
	_text.push_back(message);
	
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	if (nocontinue) {
		Input::set_mode(Input::NONE);
	}
	else {
		Input::set_mode(Input::ENTER_TO_CONTINUE);
	}
}

void Message_Box::draw(bool force) {
	GUI::draw(force);
	if (Input::get_mode() == Input::NORMAL) {
		Message_Box::~Message_Box();
	}
}

//GUI(0, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>())
Log::Log() : GUI(GameObjects::screen_width - 30, 0, 30, GameObjects::screen_height / 2, std::vector<Text>()) {
	Text name = { 1, 0, 3, 1, "Log", TCODColor::red };
	_text.push_back(name);
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

void Log::message(const std::string& message, TCODColor color) {
	for (int i = 1; i < _text.size(); i++) {
		_text[i].y--;
		if (_text[i].y <= 0) {
			_text.erase(_text.begin() + i);
		}
	}
	int h = 1;
	int str_s = message.length();
	while (true) {
		str_s -= _width - 4;
		if (str_s <= 0) {
			break;
		}
		h++;
	}
	Text m = { 1, _height - 2, _width - 2, h, "> " + message, color };
	_text.push_back(m);
	while (_text.back().y + _text.back().h > _height - 1) {
		for (int i = 1; i < _text.size(); i++) {
				_text[i].y -= m.h;
				if (_text[i].y <= 0) {
					_text.erase(_text.begin() + i);
				}
		}
	}
	_update = true;
}

//GUI(GameObjects::screen_width / 2, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>())
Status::Status(Player* player, Time* time) : GUI(GameObjects::screen_width - 30, GameObjects::screen_height / 2, 30, (GameObjects::screen_height / 2) + 1, std::vector<Text>()) {
	Text name = { 1, 0, 6, 1, "Status", TCODColor::red };
	Text texttime = { 1, 2, 20, 1, "Time: Unknown", TCODColor::red };
	Text healthsep1 = { 1, 4, _width - 1, 1, "Health" + std::string(_width - 8, '-'), TCODColor::white };
	Text headh = { 1, 5, 4, 1, "Head", TCODColor::green };
	Text leftarmh = { 1, 6, 8, 1, "Left Arm", TCODColor::green };
	Text torsoh = { 10, 6, 5, 1, "Torso", TCODColor::green };
	Text rightarmh = { 16, 6, 9, 1, "Right Arm", TCODColor::green };
	Text leftlegh = { 1, 7, 8, 1, "Left Leg", TCODColor::green };
	Text rightlegh = { 10, 7, 9, 1, "Right Leg", TCODColor::green };
	Text healthsep2 = { 1, 8, _width - 1, 1, std::string(_width - 2, '-'), TCODColor::white };

	_text.push_back(name);
	_text.push_back(texttime);
	_text.push_back(healthsep1);
	_text.push_back(headh);
	_text.push_back(leftarmh);
	_text.push_back(torsoh);
	_text.push_back(rightarmh);
	_text.push_back(leftlegh);
	_text.push_back(rightlegh);
	_text.push_back(healthsep2);

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	_player = player;
	_time = time;
	_update = true;
}

void Status::draw(bool force) {
	if (force) {
		if (_player != nullptr || _player != NULL) {
			if (_player->is_item_in_inventory("Digital Watch")) {
				_text[1].str = "Time: " + _time->format_time("%M/%D/%Y %H:%m:%S");
				_text[1].color = TCODColor::green;
			}
			else {
				_text[1].str = "Time: Unknown";
				_text[1].color = TCODColor::red;
			}
		}
	}
	GUI::draw(force);
}

SelectionBox::SelectionBox() {
	if (_transparency != 1.0f) {
		make_transparency_work();
	}
}

SelectionBox::SelectionBox(int x, int y, int w, int h, std::vector<Text> text) : GUI(x, y, w, h, text) {
	if (_transparency != 1.0f) {
		make_transparency_work();
	}
}

void SelectionBox::draw(bool force) {
	
	if (_update || force) {
		
		GUI::draw(force);
		
		for (int i = 0; i < _mtext.size(); i++) {
			_cons->setColorControl(TCOD_COLCTRL_1, _mtext[i].color, TCODColor::black);
			_cons->printRect(_mtext[i].x, _mtext[i].y, _mtext[i].w, _mtext[i].h, (std::string("%c") + _mtext[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
		}
		
		if (_transparency != 1.0f) {
			make_transparency_work();
		}
		TCODConsole::blit(_cons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, _transparency);
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
				_update = true;
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
				_update = true;
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

SelectionBoxEx::SelectionBoxEx(int x, int y, int w, int h) : SelectionBox(x, y, w, h, std::vector<Text>()) {}

void SelectionBoxEx::update_mtext() {
	for (int i = 0; i < _mtext.size(); i++) {
		_mtext[i].str.insert(0, 1, alphabet[i]);
		_mtext[i].str.insert(1, " - ");
		std::cout << _mtext[i].str << std::endl;
	}
}

MainMenu::MainMenu() : SelectionBox(0, 0, GameObjects::screen_width, GameObjects::screen_height, std::vector<Text>()) {
	front();
}

void MainMenu::front() {
	_text.clear();
	_mtext.clear();

	Text title = { (_width / 2) - 50, 5, 100, 50, 
		"`#######                                                `#####    `##\n"
		"`##    `##                                            `##    `##  `##\n"
		"`##    `##     `##       `##   `##  `##  `###          `##      `######   `###     `### `####\n"
		"`##`##       `##  `##  `##  `##`##  `## `#  `##          `##      `##   `##  `##  `##   `##\n"
		"`##  `##    `##    `##`##   `##`##  `##`##### `#            `##   `##  `##   `##  `##     `###\n"
		"`##    `##   `##  `##  `##  `##`##  `##`#             `##    `##  `##  `##   `##  `##       `##\n"
		"`##      `##   `##         `##   `##`## `#####          `#####     `##   `## `###`###   `## `##\n"
		"`##                       `##                                                             `##\n",
		TCODColor::red };
	_text.push_back(title);

	Text pic = { (_width / 2) - 5, 14, 20, 15,
"     #     \n"
"     #     \n"
"    ###    \n"
"   #####   \n"
"###########\n"
" ######### \n"
"  #######  \n"
"   #####   \n"
"   #####   \n"
"   ## ##   \n"
"  ##   ##  \n",
	TCODColor::white };

	_text.push_back(pic);

	MText n1 = { (_width / 2) - 5, 28, 20, 1, "New Game", TCODColor::white, 1, GameEvent::STARTUP_NEW_GAME };
	MText n2 = { (_width / 2) - 5, 30, 20, 1, "Load Game", TCODColor::white, 0, GameEvent::SAVE_SCREEN };
	MText n3 = { (_width / 2) - 5, 32, 20, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };
	
	_mtext.push_back(n1); _mtext.push_back(n2); _mtext.push_back(n3);

	set_selector();
	Input::set_mode(Input::NONE);
}

void MainMenu::save_screen() {
	_mtext.clear();
	
	MText back = { (_width / 2) - 5, 28, 20, 1, "Back", TCODColor::white, 1, GameEvent::TO_MAIN_MENU };
	_mtext.push_back(back);

//	MText ng = { (_width / 2) - 5, 28, 20, 1, "New Save", TCODColor::white, 1, GameEvent::STARTUP_NEW_GAME };
//	_mtext.push_back(ng);

	int y = 29;
	try {
		for (auto& f : std::experimental::filesystem::directory_iterator("Data\\Save")) {
			if (std::experimental::filesystem::is_directory(f.status())) {
				MText s = { (_width / 2) - 5, y, 20, 1, f.path().string(), TCODColor::white, 0, GameEvent::STARTUP_LOAD_GAME };
				_mtext.push_back(s);
				y++;
			}
			std::cout << f << std::endl;
		}
	}
	catch (std::experimental::filesystem::filesystem_error e) {
		std::cerr << e.what() << std::endl;
	}
	set_selector();
	Input::set_mode(Input::NONE);

	_cons->clear();
}

void MainMenu::draw(bool force) {
	_update = true;
	SelectionBox::draw(force);
	if (TCODSystem::getFps() != 0) {
		if (GameObjects::ticks % 180 == 0 && _state == FRONT) {
			_text[1].color = TCODColor(std::cos(GameObjects::time) * 255, std::sin(GameObjects::time) * 255, std::tan(GameObjects::time) * 255);
		}
	}
	if (Input::get_last_key().shift && Input::get_last_key().vk == TCODK_1) {
		GameEvent::set_event(GameEvent::TEST_LEVEL);
	}
}

ESCMenu::ESCMenu() : SelectionBox(GameObjects::screen_width / 2 - 13, GameObjects::screen_height / 2 - 10, 24, 12, std::vector<Text>()) {
	MText n1 = { (_width / 2) - 5, 2, _width / 2, 1, "Main Menu", TCODColor::white, 1, GameEvent::TO_MAIN_MENU };
	MText n2 = { (_width / 2) - 5, 3, _width / 2, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };

	_mtext.push_back(n1); _mtext.push_back(n2);

	set_selector();
	Input::set_mode(Input::ESC);
}

InfoViewer::InfoViewer(Actor* aref) : GUI(0, (GameObjects::screen_height - 10), 20, 10, std::vector<Text>()) {
	Text title = { (_width / 2) - 4, 0, _width / 2, 1, "Info", TCODColor::red };
	Text c1 = { 2, 2, _width - 2, 1, "", TCODColor::white };
	Text c2 = { 2, 3, _width - 2, 1, "", TCODColor::white };
	Text c3 = { 2, 4, _width - 2, 1, "", TCODColor::white };
	Text c4 = { 2, 5, _width - 2, 1, "", TCODColor::white };
	Text c5 = { 2, 6, _width - 2, 1, "", TCODColor::white };
	Text c6 = { 2, 7, _width - 2, 1, "", TCODColor::white };
_text.push_back(title);
_text.push_back(c1); _text.push_back(c2); _text.push_back(c3);
_text.push_back(c4); _text.push_back(c5); _text.push_back(c6);

_type = FILLED_BORDERED_BACKGROUND;
_transparency = 0.9;
_actor = aref;
}

void InfoViewer::draw(bool force) {
	if (_update || force) {
		GUI::draw(force);
		std::vector<Actor*> actors = Actor::get_actors(_actor->get_world_pos()[0], _actor->get_world_pos()[1], 0);

		if (actors.size() == 0) {
			for (int i = 1; i < _text.size() - 1; i++) {
				_text[i].str = "";
				return;
			}
		}

		for (int i = actors.size() - 1, j = 1;
			i >= 0 && j < _text.size();
			i--, j++) {

			Actor* a = actors[i];

			if (!a->is_in_fov()) {
				_text[j].str = "";
				continue;
			}

			char c = a->get_char();
			std::string name = a->get_name();

			_text[j].str = std::string(1, c) + " " + name;
		}
		for (int i = _text.size() - 1; i > actors.size(); i--) {
			_text[i].str = "";
		}
	}
}

InventoryPanel::InventoryPanel(Character* c) : SelectionBoxEx(0, 0, GameObjects::screen_width - 30, GameObjects::screen_height) {
	Text title = { (GameObjects::screen_width - 31) - c->get_name().length() - 10, 0, 30, 1, c->get_name() + " Inventory", TCODColor::red };
	_text.push_back(title);

	std::vector<Item*>* inv = c->get_inventory();

	int ypos = 2;
	for (Item* i : (*inv)) {
		MText item = { 3, ypos, 50, 1, i->get_name(), TCODColor::white };
		_mtext.push_back(item);
		ypos++;
		std::cout << _mtext.size() << " " << i->get_name() << std::endl;
	}

	if (_mtext.size() > 0) {
		_mtext[0].selected = true;
	}

	set_selector();
	update_mtext();

	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0.9;
}

Map::Map(Level* level) : GUI(0, 0, GameObjects::screen_width - 30, GameObjects::screen_height, std::vector<Text>()) {
	Text title = { _width - 4, 0, 30, 1, "Map", TCODColor::red };
	_text.push_back(title);

	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0.9;

	// Generate Map

	int maparea_x = 1;
	int maparea_y = 1;
	int maparea_w = _width - 2;
	int maparea_h = _height - 2;

	std::vector<std::vector<std::vector<Actor*>>>* data = level->get_actor_map();

	std::vector<std::vector<TCODColor>> colormap;
	colormap.resize(level->get_size() / 2);
	for (int i = 0; i < colormap.size(); i++) {
		colormap[i].resize(level->get_size() / 2);
	}
	for (int x = 0; x < level->get_size() / 2; x++) {
		for (int y = 0; y < level->get_size() / 2; y++) {
			colormap[x][y] = (*data)[x][y].back()->get_color_obj();
		}
	}

	// Make it bigger
	
	int prev_x = colormap.size();
	int prev_y = colormap[0].size();

	std::vector<std::vector<TCODColor>> image;
	image.resize(maparea_w);
	for (int x = 0; x < image.size(); x++) {
		image[x].resize(maparea_h);
	}
	std::cout << "Map x size: " << colormap.size() << std::endl;
	std::cout << "Map y size: " << colormap[0].size() << std::endl;
	
	for (int x = 0; x < maparea_w; x++) {
		for (int y = 0; y < maparea_h; y++) {
			image[x][y] = colormap[x*prev_x/maparea_w][y*prev_y/maparea_h];
		}
	}
	
	// Display to screen

	for (int x = 0; x < image.size(); x++) {
		for (int y = 0; y < image[x].size(); y++) {
			_cons->putCharEx(x + maparea_x, y + maparea_y, 0, TCODColor::black, image[x][y]);
		}
	}

}

Map::~Map() {
	TCODConsole::root->clear();
}
