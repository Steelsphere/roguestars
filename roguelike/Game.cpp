#include "Game.h"

Game::Game() : _screen_width(GameObjects::screen_width), _screen_height(GameObjects::screen_height), _time(0)
{
}


Game::~Game()
{
}

void Game::init() {
	TCODConsole::setCustomFont("terminal12x12_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(_screen_width, _screen_height, "Roguelike", false, TCOD_RENDERER_GLSL);
//	TCODSystem::setFps(60);
}

void Game::start() {
	TCODConsole::root->clear();

	_level->generate_level(512, 512, Level::HILLS);
	
	GameObjects::player = new Player(250, 250, 0, '@', TCODColor::blue);
	GameObjects::player->spawn_player_in_world();
	GameObjects::camera = new Camera(GameObjects::player);
	GameObjects::camera->set_level(_level);
	GameObjects::camera->update();

	game_loop();
}

void Game::game_loop() {
	while (true) {
		_event = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &_key, &_mouse);
		
		if (_event) {
			on_event(_event);
		}
		
		if (TCODConsole::isWindowClosed()) {
			on_exit();
		}
		
		if (GameObjects::update) {
			update();
		}
		
		TCODConsole::flush();
		std::cout << "FPS: " << TCODSystem::getFps() << "\r";
		_time += 0.01f;
	}
}

void Game::on_event(TCOD_event_t e) {
	Input::input(_key);
}

void Game::on_exit() {
	exit(0);
}

void Game::update() {
	_num_actors_drawn = 0;
	std::vector<Actor*>* actors = _level->get_actors();
	
	Actor* a;
	int* ar, *cr;
	
	GameObjects::camera->update();
	
	for (int i = 0; i < actors->size(); i++) {
		
		a = actors->at(i);
		
		ar = a->get_screen_pos();
		cr = GameObjects::camera->get_screen_pos();
		
		if (ar[0] < cr[0] + _screen_width / 2 + 10 &&
			ar[1] < cr[1] + _screen_height / 2 + 10 &&
			ar[0] > cr[0] - _screen_width / 2 - 10 &&
			ar[1] > cr[1] - _screen_height / 2 - 10
			) {
				_num_actors_drawn++;
				a->draw();
			} 
	}
//	std::cout << "Number of actors drawn " << _num_actors_drawn << "\r";
	GameObjects::update = false;
}


