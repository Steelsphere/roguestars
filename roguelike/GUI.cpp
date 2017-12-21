#include "GUI.h"
#include "GameObjects.h"
#include "Input.h"
#include "Actor.h"
#include "Player.h"
#include "Faction.h"
#include "Tile.h"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <iostream>
#include <filesystem>
#include <tuple>
#include <ctype.h>

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
//	std::cout << "GUIs: " << _buffer.size() << std::endl;
}

void GUI::draw(bool force) {
	if (_update || force) {
		
		// Add background
		switch (_type) {
		case FILLED_BACKGROUND:
			_cons->rect(0, 0, _width, _height, true);
			break;
		case FILLED_BORDERED_BACKGROUND:
			_cons->printFrame(0, 0, _width, _height, true);
			break;
		}
		
		// Add text
		for (int i = 0; i < _text.size(); i++) {
			_cons->setColorControl(TCOD_COLCTRL_1, _text[i].color, _text[i].bcolor);
			if (!_text[i].ovrcolor) {
				_cons->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, (std::string("%c") + _text[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
			}
			else {
				_cons->printRect(_text[i].x, _text[i].y, _text[i].w, _text[i].h, _text[i].str.c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
			}
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

		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				screen[x][y].push_back(std::make_tuple(TCODConsole::root->getChar(_x + x, _y + y),
					TCODConsole::root->getCharForeground(_x + x, _y + y),
					TCODConsole::root->getCharBackground(_x + x, _y + y)));
			}
		}
		_screencons = new TCODConsole(_width, _height);
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				if (std::get<0>(screen[x][y][0]) >= 0) {
					_screencons->putCharEx(x, y, std::get<0>(screen[x][y][0]), std::get<1>(screen[x][y][0]), std::get<2>(screen[x][y][0]));
				}
			}
		}
		TCODConsole::blit(_screencons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, 1.0f);
	}
	else {
		TCODConsole::blit(_screencons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, 1.0f);
	}
}

void GUI::clear_working_area() {
	for (int x = _x; x < _width; x++) {
		for (int y = _y; y < _height; y++) {
			TCODConsole::root->putChar(x, y, ' ');
		}
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

Log::Log(int x, int y, int w, int h) : GUI(x, y, w, h, std::vector<Text>()) {
	Text name = { 1, 0, 3, 1, "Log", TCODColor::red };
	_text.push_back(name);
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

void Log::message(const std::string& message, TCODColor color) {
	_cons->clear();
	clear_working_area();
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
	draw();
	TCODConsole::flush();
}

//GUI(GameObjects::screen_width / 2, GameObjects::screen_height - GameObjects::screen_height / 8, GameObjects::screen_width / 2, GameObjects::screen_width / 11, std::vector<Text>())
Status::Status(Player* player, Time* time) : GUI(GameObjects::screen_width - 30, GameObjects::screen_height / 2, 30, (GameObjects::screen_height / 2) + 1, std::vector<Text>()) {
	Text name = { 1, 0, 6, 1, "Status", TCODColor::red };
	
	_health = new HealthInfo(1 + _x, 1 + _y, _width - 2, 5, player);
	_tile = new TileInfo(1 + _x, 6 + _y, _width - 2, 8, player);
	_char = new CharInfo(1 + _x, 14 + _y, _width - 2, 16, player);
	_misc = new MiscInfo(1 + _x, 30 + _y, _width - 2, 7, player);

	_text.push_back(name);
	
	_transparency = 0.0f;
	_type = FILLED_BORDERED_BACKGROUND;
	_player = player;
	_time = time;
	_update = true;
}

void Status::draw(bool force) {
	if (force) {
		/*if (_player != nullptr || _player != NULL) {
			if (_player->is_item_in_inventory("Digital Watch")) {
				_text[1].str = "Time: " + _time->format_time("%M/%D/%Y %H:%m:%S");
				_text[1].color = TCODColor::green;
			}
			else {
				_text[1].str = "Time: Unknown";
				_text[1].color = TCODColor::red;
			}
		} */
		if (_player != nullptr) {
			_tile->update();
			
			// Sector view
			std::vector<Actor*> tiles = Actor::get_actors(_player->get_world_pos()[0], _player->get_world_pos()[1], 0);
			StarSector* sector = nullptr;
			for (Actor* a : tiles) {
				if (a->get_type() == typeid(StarSector).name()) {
					sector = dynamic_cast<StarSector*>(a);
				}
			}
			if (sector != nullptr) {
				if (_si == nullptr) {
					_si = new SectorInfo(_x - 25, _y, 25, 12, sector);
				}
				else if (sector != _si->get_sector()) {
					delete _si;
					_si = new SectorInfo(_x - 25, _y, 25, 12, sector);
				}
			}
			else if (_si != nullptr) {
				delete _si;
				_si = nullptr;
			}

		}
	} 
	GUI::draw(force);
}

Status::~Status() {
	delete _health;
	delete _tile;
	delete _char;
	delete _misc;
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

SelectionBox::SelectionBox(int x, int y, int w, int h, std::vector<MText> mtext) : GUI(x, y, w, h) {
	if (_transparency != 1.0f) {
		make_transparency_work();
	}
	_mtext = mtext;
}

void SelectionBox::draw(bool force) {
	
	if (_update || force) {
		
		GUI::draw(force);
		
		for (int i = 0; i < _mtext.size(); i++) {
			_cons->setBackgroundFlag(TCOD_BKGND_SET);
			_cons->setColorControl(TCOD_COLCTRL_1, _mtext[i].color, _mtext[i].bcolor);
			_cons->printRect(_mtext[i].x, _mtext[i].y, _mtext[i].w, _mtext[i].h, (std::string("%c") + _mtext[i].str + std::string("%c")).c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
			_cons->root->setBackgroundFlag(TCOD_BKGND_NONE);
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
				set_selector(true);
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
				set_selector(true);
				_update = true;
				break;
			}
		}
	}
	if (Input::get_last_key().vk == TCODK_ENTER && Input::get_last_key().pressed) {
		for (int i = 0; i < _mtext.size(); i++) {
			if (_mtext[i].selected) {
				selection = true;
				GameEvent::set_event(_mtext[i].action);
			}
		}
	}
	for (MText& m : _mtext) {
		int x = Input::get_last_mouse().cx;
		int y = Input::get_last_mouse().cy;
		if (x >= (m.x + _x) && x < (m.x + _x) + m.str.length() &&
			y >= (m.y + _y) && y < (m.y + _y) + m.h) {
			m.bcolor = TCODColor::blue;
			if (Input::get_last_mouse().lbutton_pressed) {
				for (MText& m : _mtext) {
					m.selected = false;
				}
				m.selected = true;
				selection = true;
				GameEvent::set_event(m.action);
			}
		}
		else {
			m.bcolor = TCODColor::black;
		}
	}
	set_selector();
}

void SelectionBox::position_text() {
	GUI::position_text();
	for (int i = 0; i < _mtext.size(); i++) {
		_mtext[i].x += _x;
		_mtext[i].y += _y;
	}
}

void SelectionBox::set_selector(bool force) {
	for (MText& i : _mtext) {
		if (i.selected) {
			if (GameObjects::ticks % 24 == 0 || force) {
				i.color = TCODColor((int)(std::sin(GameObjects::ticks) * 100) + 150, (int)(std::sin(GameObjects::ticks) * 100) + 150, 0);
			}
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

	Text title = { (_width / 2) - 50, 5, 97, 10, 
		"`#######                                                `#####    `##                          \n"
		"`##    `##                                            `##    `##  `##                          \n"
		"`##    `##     `##       `##   `##  `##  `###          `##      `######   `###     `### `####  \n"
		"`##`##       `##  `##  `##  `##`##  `## `#  `##          `##      `##   `##  `##  `##   `##    \n"
		"`##  `##    `##    `##`##   `##`##  `##`##### `#            `##   `##  `##   `##  `##     `### \n"
		"`##    `##   `##  `##  `##  `##`##  `##`#             `##    `##  `##  `##   `##  `##       `##\n"
		"`##      `##   `##         `##   `##`## `#####          `#####     `##   `## `###`###   `## `##\n"
		"`##                       `##                                                             `##  \n",
		TCODColor::red };
	_title_pic = new Picture(title);

	Text pic = { (_width / 2) - 5, 15, 13, 20,
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
	TCODColor::orange };

	_star_pic = new Picture(pic);

	MText n1 = { (_width / 2) - 4, 28, 20, 1, "New Game", TCODColor::white, 1, GameEvent::STARTUP_NEW_GAME };
	MText n2 = { (_width / 2) - 4, 30, 20, 1, "Load Game", TCODColor::white, 0, GameEvent::SAVE_SCREEN };
	MText n3 = { (_width / 2) - 4, 32, 20, 1, "Exit", TCODColor::white, 0, GameEvent::EXIT };
	
	_mtext.push_back(n1); _mtext.push_back(n2); _mtext.push_back(n3);

	_type = FILLED_BORDERED_BACKGROUND;

	set_selector();
	Input::set_mode(Input::MAIN_MENU);
}

void MainMenu::save_screen() {
	delete _title_pic;
	delete _star_pic;
	
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
	Input::set_mode(Input::MAIN_MENU);

	_cons->clear();
}

void MainMenu::draw(bool force) {
	if (Input::get_mode() == Input::MAIN_MENU) {
		_update = true;
		SelectionBox::draw(force);
		if (TCODSystem::getFps() != 0) {
			if (GameObjects::ticks % 180 == 0 && _state == FRONT) {
	//			_text[1].color = TCODColor(std::cos(GameObjects::time) * 255, std::sin(GameObjects::time) * 255, std::tan(GameObjects::time) * 255);
			}
		}
		if (Input::get_last_key().shift && Input::get_last_key().vk == TCODK_1) {
			GameEvent::set_event(GameEvent::TEST_LEVEL);
		}
	}
}

MainMenu::~MainMenu() {
	delete _title_pic;
	delete _star_pic;
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
	Text title = { 1, 0, 30, 1, c->get_name() + " Inventory", TCODColor::red };
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

Map::Map(Level* level, bool background, Player* player) : GUI(0, 0, GameObjects::screen_width - 30, GameObjects::screen_height, std::vector<Text>()) {
	Text title = { 1, 0, 30, 1, "Map", TCODColor::red };
	_text.push_back(title);

	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0.9;
	
	// Generate Map

	update_map(level, background, player);

}

Map::Map(int x, int y, int w, int h, Level* level, bool background) : GUI(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, 30, 1, "Map", TCODColor::red };
	_text.push_back(title);

	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0.9;

	// Generate Map

	update_map(level, background);
}

Map::~Map() {
	TCODConsole::root->clear();
}

void Map::update_map(Level* level, bool background, Player* player) {
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
			if (background) {
				colormap[x][y] = (*data)[x][y].back()->get_bcolor_obj();
			}
			else {
				colormap[x][y] = (*data)[x][y].back()->get_color_obj();
			}
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
//	std::cout << "Map x size: " << colormap.size() << std::endl;
//	std::cout << "Map y size: " << colormap[0].size() << std::endl;

	for (int x = 0; x < maparea_w; x++) {
		for (int y = 0; y < maparea_h; y++) {
			image[x][y] = colormap[x*prev_x / maparea_w][y*prev_y / maparea_h];
		}
	}

	// Add player

	int px = 0;
	int py = 0;
	if (player != nullptr) {
		px = player->get_world_pos()[0] * ((float)maparea_w / (float)prev_x);
		py = player->get_world_pos()[1] * ((float)maparea_h / (float)prev_y);
	}

	// Display to screen

	_cons->clear();
	for (int x = 0; x < image.size(); x++) {
		for (int y = 0; y < image[x].size(); y++) {
			_cons->putCharEx(x + maparea_x, y + maparea_y, 0, TCODColor::black, image[x][y]);
		}
	}

	if (player != nullptr) {
		Text pt = { px + maparea_x, py + maparea_y, 1, 1, "@", TCODColor::blue };
		_text.push_back(pt);
	}
}

LoadingScreen::LoadingScreen(const std::string& text) : GUI(0, 0, GameObjects::screen_width, GameObjects::screen_height, std::vector<Text>()) {
	Text loading = { (_width / 2) - 5, _height - 7, 20, 1, "Loading...", TCODColor::white };
	Text bottext = { (_width / 2) - 25, _height - 5, 60, 1, text, TCODColor::white };

	_text.push_back(loading);
	_text.push_back(bottext);

	_type = FILLED_BORDERED_BACKGROUND;
	_transparency = 0;

	draw(true);
}

void LoadingScreen::set_text(const std::string& text) {
	_text[1].str = text;
	draw(true);
}

HealthInfo::HealthInfo(int x, int y, int w, int h, Player* p) : GUI(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, w, 1, "Health", TCODColor::red };
	Text headh = { 10, 1, 4, 1, "Head", TCODColor::green };
	Text leftarmh = { 1, 2, 8, 1, "Left Arm", TCODColor::green };
	Text torsoh = { 10, 2, 5, 1, "Torso", TCODColor::green };
	Text rightarmh = { 16, 2, 9, 1, "Right Arm", TCODColor::green };
	Text leftlegh = { 1, 3, 8, 1, "Left Leg", TCODColor::green };
	Text rightlegh = { 16, 3, 9, 1, "Right Leg", TCODColor::green };
	
	_text.push_back(title);
	_text.push_back(headh);
	_text.push_back(leftarmh);
	_text.push_back(torsoh);
	_text.push_back(rightarmh);
	_text.push_back(leftlegh);
	_text.push_back(rightlegh);

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

TileInfo::TileInfo(int x, int y, int w, int h, Player* p) : GUI(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, w, 1, "Tile", TCODColor::red };
	
	Text tname = { 1, 1, w - 2, 1, "Name: %cTest%c", TCODColor::red, true };
	Text tdesc = { 1, 2, w - 2, 1, "Description: ", TCODColor::white, true };
	Text tfact = { 1, 3, w - 2, 1, "Faction: ", TCODColor::white, true };
	Text tappr = { 1, 4, w - 2, 1, "Appearance: ", TCODColor::white, true };
	Text timpa = { 1, 5, w - 2, 1, "Impassable: ", TCODColor::white, true };
	Text ttran = { 1, 6, w - 2, 1, "Transparent: ", TCODColor::white, true };

	_text.push_back(title);

	_text.push_back(tname);
	_text.push_back(tdesc);
	_text.push_back(tfact);
	_text.push_back(tappr);
	_text.push_back(timpa);
	_text.push_back(ttran);

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	_player = p;
}

void TileInfo::update() {
	auto v = Actor::get_actors(_player->get_world_pos()[0], _player->get_world_pos()[1], 0);
	if (v.size() < 2) {
		_text[1].str = "Name: ";
		_text[2].str = "Description: ";
		_text[3].str = "Faction: ";
		_text[4].str = "Appearance: ";
		_text[5].str = "Impassable: ";
		_text[6].str = "Transparent: ";
	}
	else {
		Actor* a = v[v.size() - 2];
		
		_text[1].str = "Name: " + std::string("%c") + a->get_name() + std::string("%c");
		_text[1].color = a->get_color_obj();
		
		if (_text[1].str.length() > _text[1].w - 2) {
			while (_text[1].str.length() > _text[1].w - 2) {
				_text[1].str.pop_back();
			}
			_text[3].str.append("%c");
		}
		
		Faction* f = Faction::who_owns_tile(a);
		if (f != nullptr) {
			_text[3].str = "Faction: " + std::string("%c") + f->get_name() + std::string("%c");
			_text[3].color = f->get_color();

			if (_text[3].str.length() > _text[3].w - 2) {
				while (_text[3].str.length() > _text[3].w - 2) {
					_text[3].str.pop_back();
				}
				_text[3].str.append("%c");
			}
		
		}
		else {
			_text[3].str = "Faction: None";
			_text[3].color = TCODColor::white;
		}
//		_text[4].str = "Appearance: " + std::string("%c") + a->get_char() + std::string("%c");
//		_text[4].color = a->get_color_obj();
		if (a->is_impassable()) {
			_text[5].str = "Impassable: %cTrue%c";
			_text[5].color = TCODColor::green;
		}
		else {
			_text[5].str = "Impassable: %cFalse%c";
			_text[5].color = TCODColor::red;
		}
		if (a->is_transparent()) {
			_text[6].str = "Transparent: %cTrue%c";
			_text[6].color = TCODColor::green;
		}
		else {
			_text[6].str = "Transparent: %False%c";
			_text[6].color = TCODColor::red;
		}
	}
}

CharInfo::CharInfo(int x, int y, int w, int h, Player* p) : GUI(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, w, 1, "Character", TCODColor::red };
	Text alias = { 1, 1, w, 1, "Alias: ", TCODColor::white };
	_text.push_back(title);
	_text.push_back(alias);

	_text[1].str += p->get_alias();

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

MiscInfo::MiscInfo(int x, int y, int w, int h, Player* p) : GUI(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, w, 1, "Misc", TCODColor::red };
	Text time = { 1, 1, w, 1, "Time: %cUnknown%c", TCODColor::red, true };
	
	_text.push_back(title);
	_text.push_back(time);

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

SectorInfo::SectorInfo(int x, int y, int w, int h, StarSector* s) : GUI(x, y, w, h) {
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	_sector = s;
	update();
}

void SectorInfo::update() {
	_cons->clear();
	_text.clear();
	
	Text title = { 1, 0, _width, 1, "Sector", TCODColor::red };
	_text.push_back(title);
	
	std::vector<std::string> names = {
		"Food    ",
		"Water   ",
		"Air     ",
		"Con.Gds ",
		"Ind.Gds ",
		"Lux.Gds ",
		"Mil.Gds ",
		"Minerals",
		"Workers ",
	};
	
	Text gtitle = { 1, 1, _width, 1, "-----Goods-----", TCODColor::white };
	_text.push_back(gtitle);

	// Goods
	int i = 0;
	std::vector<int> supply = _sector->economy.supply.get_vals();
	std::vector<int> demand = _sector->economy.demand.get_vals();
	for (int y = 2; y < _height - 1; y++) {
		
		Text good = { 1, y, _width, 1, names[i], TCODColor::white};
		good.str += ":";
		good.str += "S:";
		good.str += std::to_string(supply[i]);
		good.str += "|D:";
		good.str += std::to_string(demand[i]);
		_text.push_back(good);

		i++;
		if (i > names.size() - 1) {
			break;
		}
	}
	
	// Buildings

	int by = _text.back().y + 1;

	Text btitle = { 1, by, _width, 1, "-----Buildings-----", TCODColor::white };
	_text.push_back(btitle);

	by++;

	for (Economy::Building* b : _sector->economy.buildings) {
		Text building = { 1, by, _width, 1, b->initial + "x" + std::to_string(b->tier), b->color };
		_text.push_back(building);

		by++;
	}

	// Clean up

	_height = _text.back().y + 2;
	delete _cons;
	_cons = new TCODConsole(_width, _height);
}

void SectorInfo::draw(bool force) {
	TCOD_mouse_t m = Input::get_last_mouse();
	if (m.cx > _x && m.cx < _x + _width &&
		m.cy > _y && m.cy < _y + _height) {
		return;
	}
	else {
		GUI::draw(force);
	}
}

TextBox::TextBox(int x, int y, int w, int h, std::string title, std::string descr, bool digits_only) : GUI(x, y, w, h, std::vector<Text>()) {
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;

	Text t = { 1, 0, w, 1, title, TCODColor::red };
	Text input = { 1, h - 2, w - 2, 1, "", TCODColor::white };
	Text dscr = { 1, h - 1, w, 1, descr, TCODColor::white };

	_text.push_back(input);
	_text.push_back(t);
	_text.push_back(dscr);
	
	_digits_only = digits_only;

	Input::set_mode(Input::TEXTBOX);
	GUI::make_transparency_work();
}

void TextBox::draw(bool force) {
	TCOD_key_t key = Input::get_last_key();
	if (key.pressed && (key.vk == TCODK_TEXT || 
						key.vk == TCODK_BACKSPACE || 
						key.vk == TCODK_ENTER)) {
		if (key.vk == TCODK_BACKSPACE) {
			if (_text[0].str.size() > 0) {
				_text[0].str.pop_back();
			}
		}
		else if (key.vk == TCODK_ENTER) {
			if (_digits_only) {
				if (std::atoi(_text[0].str.c_str()) > 1000000) {
					_text[0].str.clear();
				}
				else {
					_val = _text[0].str;
					_done = true;
				}
			}
			else {
				_val = _text[0].str;
				_done = true;
			}
		}
		else if (_text[0].str.size() < _text[0].w - 1) {
			if (_digits_only) {
				if (isdigit(key.c)) {
					_text[0].str += key.c;
				}
			}
			else {
				if (isalpha(key.c) || isdigit(key.c)) {
					if (key.shift) {
						_text[0].str += toupper(key.c);
					}
					else {
						_text[0].str += key.c;
					}
				}
			}
		}
	}
	
	GUI::draw(true);
	GUI::make_transparency_work();

	if (GameObjects::ticks % 50 < 25) {
		_cons->putChar(_text[0].str.size() + 1, _text[0].y, '\\');
	}
	else {
		_cons->putChar(_text[0].str.size() + 1, _text[0].y, '/');
	}

	TCODConsole::blit(_cons, 0, 0, _width, _height, TCODConsole::root, _x, _y, 1.0f, _transparency);
}

Picture::Picture(Text t) : GUI(t.x - 1, t.y - 1, t.w, t.h) {
	t.x = 1;
	t.y = 1;
	_text.push_back(t);
	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
	GUI::draw(true);
}

GalaxySizeSelection::GalaxySizeSelection(int x, int y, int w, int h) : SelectionBox(x, y, w, h, std::vector<Text>()) {
	Text title = { 1, 0, w - 1, 1, "Galaxy Size", TCODColor::white };
	_text.push_back(title);
	
	SelectionBox::MText small = { 3, 2, w - 1, 1, "Small - 128x128", TCODColor::white, true, GameEvent::NONE };
	SelectionBox::MText medium = { 3, 4, w - 1, 1, "Medium - 256x256", TCODColor::white, false, GameEvent::NONE };
	SelectionBox::MText large = { 3, 6, w - 1, 1, "Large - 512x512", TCODColor::white, false, GameEvent::NONE };

	_mtext.push_back(small);
	_mtext.push_back(medium);
	_mtext.push_back(large);

	_transparency = 0.9f;
	_type = FILLED_BORDERED_BACKGROUND;
}

void GalaxySizeSelection::draw(bool force) {
	SelectionBox::draw(force);
	if (selection) {
		if (_mtext[0].selected) {
			GameObjects::galaxy_size = GameObjects::SMALL;
			std::cout << "Small galaxy\n";
			GameObjects::log->message("Galaxy size: Small", TCODColor::white);
		}
		if (_mtext[1].selected) {
			GameObjects::galaxy_size = GameObjects::MEDIUM;
			std::cout << "Medium galaxy\n";
			GameObjects::log->message("Galaxy size: Medium", TCODColor::white);
		}
		if (_mtext[2].selected) {
			GameObjects::galaxy_size = GameObjects::LARGE;
			std::cout << "Large galaxy\n";
			GameObjects::log->message("Galaxy size: Large", TCODColor::white);
		}
	}
}