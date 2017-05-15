#include "Game.h"
#include "Camera.h"
#include "Input.h"
#include "GameEvent.h"

Game::Game() 
	: _screen_width(GameObjects::screen_width), _screen_height(GameObjects::screen_height), _time(0)
{
}


Game::~Game()
{
}

void Game::init() {
	TCODConsole::setCustomFont("terminal12x12_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(_screen_width, _screen_height, "Roguelike", false, TCOD_RENDERER_GLSL);
}

void Game::start() {
	TCODConsole::root->clear();

	_main_menu = std::shared_ptr<Main_Menu>(new Main_Menu);

	game_loop();
}

void Game::game_loop() {
	while (true) {
		_event = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &_key, &_mouse);
		
		if (_event) {
			on_event(_event);
		}
		
		if (GameEvent::get_event() != GameEvent::NONE) {
			game_event();
		}

		if (TCODConsole::isWindowClosed()) {
			exit_game();
		}
		
		update_gui();
		
		if (GameObjects::update) {
			update();
		}
		
		Input::refresh();
		
		TCODConsole::flush();
		
		TCODConsole::root->print(0, 0, (std::string("FPS: ") + std::to_string(TCODSystem::getFps())).c_str());
		_time += 0.01f;
	
	}
}

void Game::on_event(TCOD_event_t e) {
	Input::input(_key);
}

void Game::game_event() {
	switch (GameEvent::get_event()) {
	case GameEvent::STARTUP_NEW_GAME:
		startup_new_game();
		break;
	case GameEvent::STARTUP_LOAD_GAME:
		startup_load_game();
		break;
	case GameEvent::EXIT:
		exit_game();
		break;
	}
	GameEvent::reset_event();
}

void Game::exit_game() {
//	if (_level != NULL) {
//		_level->save_level_file("Data\\datalevelcompr_1.dat");
//	}
	exit(0);
}

void Game::update() {
	_num_actors_drawn = 0;
	std::vector<Actor*>* actors = _level->get_actors();
	TCODMap* fov = nullptr;

	Actor* a;
	int* ar, *cr;
	
	if (_camera != nullptr) {
		_camera->update();
	}
	
	if (_level != nullptr) {
		fov = _level->get_fov_map();
		fov->computeFov(_player->get_world_pos()[0], _player->get_world_pos()[1], 100);
	}

	if (_level != nullptr) {
		for (int i = 0; i < actors->size(); i++) {

			a = actors->operator[](i);

			ar = a->get_screen_pos();
			cr = _camera->get_screen_pos();

			if (ar[0] < cr[0] + _screen_width / 2 + 10 &&
				ar[1] < cr[1] + _screen_height / 2 + 10 &&
				ar[0] > cr[0] - _screen_width / 2 - 10 &&
				ar[1] > cr[1] - _screen_height / 2 - 10
				) {
				if (fov->isInFov(a->get_world_pos()[0], a->get_world_pos()[1])) {
					_num_actors_drawn++;
					a->draw();
				}
			}
		}
	}
//	std::cout << "Number of actors drawn " << _num_actors_drawn << "\r";
	
	_num_updates++;
	
	if (_log != nullptr) {
		_log->message(std::to_string(_num_updates), TCODColor::white);
	}
	
	GameObjects::update = false;
}

void Game::update_gui() {
	std::vector<GUI*>* guis = GUI::get_buffer();
	for (int i = 0; i < guis->size(); i++) {
		(*guis)[i]->draw();
	}
}

void Game::startup_new_game() {
	_main_menu.get()->~Main_Menu();
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;

	_world = new World;
	_world->generate_world();
	_level = _world->get_current_level();

	_player = new Player(250, 250, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();

	Input::set_mode(Input::NORMAL);
	Input::set_input_reciever(_player);
	
	_camera = new Camera(_player);
	_camera->set_level(_level);
	_camera->update();

	_log = new Log;
	_status = new Status;

}

void Game::startup_load_game() {
	_main_menu.get()->~Main_Menu();
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;

	_level = Level::load_level_file("Data\\datalevelcompr_1.dat");
	
	_player = new Player(250, 250, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();

	Input::set_input_reciever(_player);
	Input::set_mode(Input::NORMAL);

	_camera = new Camera(_player);
	_camera->set_level(_level);
	_camera->update();

	_log = new Log;
	_status = new Status;

}


