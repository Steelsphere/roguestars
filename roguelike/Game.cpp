#include "Game.h"
#include "Camera.h"
#include "Input.h"
#include "GameEvent.h"
#include "AI.h"
#include "Character.h"
#include "Structure.h"

Game::Game() 
	: _screen_width(GameObjects::screen_width), _screen_height(GameObjects::screen_height)
{
}


Game::~Game()
{
}

void Game::init() {
	TCODConsole::setCustomFont("terminal12x12_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(_screen_width, _screen_height, "Rogue Stars", false, TCOD_RENDERER_GLSL);
	TCODSystem::setFps(180);
}

void Game::start() {
	TCODConsole::root->clear();
	new_main_menu();
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
		
		if (GameObjects::update) {
			update();
		}
		
		update_gui();
		
		Input::refresh();
		
		TCODConsole::flush();
		
		TCODConsole::root->print(0, 0, (std::string("FPS: ") + std::to_string(TCODSystem::getFps())).c_str());
		GameObjects::time += 0.01f;
		GameObjects::ticks++;

	}
}

void Game::on_event(TCOD_event_t e) {
	Input::input(_key);
}

void Game::game_event() {
	switch (GameEvent::get_event()) {
	case GameEvent::TO_MAIN_MENU:
		new_main_menu();
		break;
	case GameEvent::STARTUP_NEW_GAME:
		startup_new_game();
		break;
	case GameEvent::STARTUP_LOAD_GAME:
		startup_load_game();
		break;
	case GameEvent::NEW_ESC_MENU:
		new_esc_menu();
		break;
	case GameEvent::DELETE_ESC_MENU:
		destroy_esc_menu();
		break;
	case GameEvent::NEW_INFO_VIEWER:
		new_info_viewer();
		break;
	case GameEvent::DELETE_INFO_VIEWER:
		destroy_info_viewer();
		break;
	case GameEvent::NEW_WORLD:
		new_world();
		break;
	case GameEvent::NEW_SOLAR_SYSTEM:
		new_solar_system();
		break;
	case GameEvent::NEW_STAR_SECTOR:
		new_star_sector();
		break;
	case GameEvent::NEW_GALAXY:
		new_galaxy();
		break;
	case GameEvent::NEW_WORLD_MAP:
		new_world_map();
		break;
	case GameEvent::ENTER_WORLD_TILE:
		enter_world_tile();
		break;
	case GameEvent::ENTER_SPACESHIP:
		enter_spaceship();
		break;
	case GameEvent::OPEN_INVENTORY:
		open_inventory();
		break;
	case GameEvent::CLOSE_INVENTORY:
		close_inventory();
		break;
	case GameEvent::UPWARDS:
		upwards();
		break;
	case GameEvent::HIGHLIGHT_PLAYER:
		highlight_player();
		break;
	case GameEvent::DEHIGHLIGHT_PLAYER:
		dehighlight_player();
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
//	if (_CrtDumpMemoryLeaks()) {
//		std::cout << "Memory leak found!\n";
//	}
	exit(0);
}

void Game::update() {
	if (GameObjects::new_turn && GameObjects::player_controlled) {
		update_characters();
		_turn++;
		
		if (_log != nullptr) {
			_log->message(std::to_string(_turn), TCODColor::white);
		}
		
		GameObjects::new_turn = false;
	}
	
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
					a->set_fov(true);
					a->set_memorization(true);
					a->draw();
				}
				else if (a->is_memorized()) {
					a->draw_mem();
				}
				else {
					a->set_fov(false);
				}
			}
		}
	}
//	std::cout << "Number of actors drawn " << _num_actors_drawn << "\r";
	
	_num_updates++;
	
	update_gui(true);

	GameObjects::update = false;
}

void Game::update_gui(bool all) {
	std::vector<GUI*>* guis = GUI::get_buffer();
	if (all) {
		for (int i = 0; i < guis->size(); i++) {
			(*guis)[i]->draw(true);
		}
	}
	else {
		for (int i = 0; i < guis->size(); i++) {
			(*guis)[i]->draw();
		}
	}
}

void Game::update_characters() {
	std::vector<Character*>* b = Character::get_chbuff();
	for (Character* c : (*b)) {
		c->update();
		_level->update_tile(c->get_world_pos()[0], c->get_world_pos()[1], 0);
	}
}

void Game::startup_new_game() {
	destroy_main_menu();
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;
	new_galaxy();
	_log = new Log;
	_status = new Status;
	new Message_Box("No errors");
}

void Game::startup_load_game() {
	destroy_main_menu();
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;
	_level = Level::load_level_file("Data\\datalevelcompr_1.dat");
	_player = new Player(250, 250, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	level_setup();
	_log = new Log;
	_status = new Status;

}

void Game::new_main_menu() {
	destroy_garbage();
	_MainMenu = new MainMenu;
	TCODConsole::root->flush();
}

void Game::destroy_main_menu() {
	delete _MainMenu;
	_MainMenu = nullptr;
}

void Game::new_esc_menu() {
	_ESCMenu = new ESCMenu;
}

void Game::destroy_esc_menu() {
	delete _ESCMenu;
	_ESCMenu = nullptr;
}

void Game::destroy_garbage() {
	delete _camera;
	delete	_log;
	delete	_status;
	delete	_MainMenu;
	delete	_ESCMenu;
	delete	_info_viewer;
	delete	_level;
	delete	_world;
	_camera = nullptr;
	_log = nullptr;
	_status = nullptr;
	_MainMenu = nullptr;
	_ESCMenu = nullptr;
	_info_viewer = nullptr;
	_level = nullptr;
	_world = nullptr;
}

void Game::new_info_viewer() {
	_dummy = new Dummy(_player->get_screen_pos()[0], _player->get_screen_pos()[1], 0, 'X', TCODColor::yellow);
	_dummy->set_world_position(_player->get_world_pos()[0], _player->get_world_pos()[1], 0);
	_info_viewer = new InfoViewer(_dummy);
	_dummy->set_info(_info_viewer);
	Input::set_input_reciever(_dummy);
	_camera->set_following(_dummy);
	GameObjects::update = true;
}

void Game::destroy_info_viewer() {
	delete _dummy;
	delete _info_viewer;
	_dummy = nullptr;
	_info_viewer = nullptr;
	Input::set_input_reciever(_player);
	_camera->set_following(_player);
	GameObjects::update = true;
}

void Game::new_world() {
	loading_screen();
	
	_world = new World(1024);
	_world->generate_world();
	_level = _world->get_current_level();
	_level->save_level_image("Data\\Level.png");
	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	
	new Monster(_player->get_screen_pos()[0]-10, _player->get_screen_pos()[1]-10, 0);
	
	level_setup();
	_lightsystem.set_global_lighting(_level, 15, 15, 0);
	
	GameObjects::update = true;
}

void Game::new_solar_system() {
	_solar_system = new Level;
	_solar_system->generate_level(256, Level::SOLAR_SYSTEM);
	
	_level = _solar_system;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	
	level_setup();

	GameObjects::update = true;
}

void Game::new_star_sector() {
	_star_sector = new Level;
	_star_sector->generate_level(512, Level::STAR_SECTOR);
	
	_level = _star_sector;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	
	level_setup();
	GameObjects::update = true;
}

void Game::new_galaxy() {
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;
	
	_galaxy = new Level;
	_galaxy->generate_level(1024, Level::GALAXY);
	
	_level = _galaxy;
	
	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_galaxy();

	_player->add_to_inventory(new Item(0, 0, 0, Item::FLOWER));
	_player->add_to_inventory(new Item(0, 0, 0, Item::FLOWER));
	_player->add_to_inventory(new Item(0, 0, 0, Item::FLOWER));
	_player->add_to_inventory(new Item(0, 0, 0, Item::FLOWER));

	level_setup();
	GameObjects::update = true;
}

void Game::new_world_map() {
	if (_level->get_type() == Level::WORLD_MAP) {
		return;
	}
	
	loading_screen();

	_lightsystem.clear_cache();
	if (_world != nullptr && _level != nullptr) {
		delete _level;
		_level = _world->generate_world_map();

		_player = new Player(_world->get_current_pos().first, _world->get_current_pos().second, 0, '@', TCODColor::blue);
		
		level_setup();
		_camera->update();
	}
	
	GameObjects::update = true;
}

void Game::enter_world_tile() {
	loading_screen();

	delete _level;
	_world->new_level(_player->get_world_pos()[0], _player->get_world_pos()[1]);
	_level = _world->get_current_level();
	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	
	new Monster(_player->get_screen_pos()[0] - 10, _player->get_screen_pos()[1] - 10, 0);

	level_setup();
	_lightsystem.set_global_lighting(_level, 15, 15, 0);
	
	GameObjects::update = true;
}

void Game::level_setup() {
	delete _camera;
	AI::setup(_player);
	Input::set_input_reciever(_player);
	Input::set_mode(Input::NORMAL);
	_camera = new Camera(_player);
	_camera->set_level(_level);
	_camera->update();
}

void Game::enter_spaceship() {
	if (_spaceship == nullptr) {
		_spaceship = new Level;
		_spaceship->generate_level(1028, Level::SPACE);
		_level = _spaceship;
		
		new Structure(216, 216, Structure::TINY_SPACESHIP);

		_player = new Player(218, 217, 0, '@', TCODColor::blue);
		
		level_setup();
		GameObjects::update = true;
	}
	else {
		_level = _spaceship;
	}
}

void Game::loading_screen() {
	Message_Box* mb = new Message_Box("Loading...", true);
	update();
	TCODConsole::flush();
	delete mb;
}

void Game::open_inventory() {
	_inv_panel = new InventoryPanel(_player);
}

void Game::close_inventory() {
	delete _inv_panel;
	_inv_panel = nullptr;
}

void Game::upwards() {
	if (_level->get_type() == Level::GALAXY) {
		return;
	}
	else if (_level->get_type() == Level::STAR_SECTOR) {
		to_galaxy();
	}
	else if (_level->get_type() == Level::SOLAR_SYSTEM) {
		to_star_sector();
	}
	else if (_level->get_type() == Level::WORLD_MAP) {
		to_solar_system();
	}
	else if (_level->get_type() != Level::SPACE) {
		new_world_map();
	}
}

void Game::to_galaxy() {
	_level = _galaxy;
	
	Actor::set_buffer(_level->get_actors());
	Actor::set_map(_level->get_actor_map());
	
	_level->update();
	
	_player = static_cast<Player*>(GameObjects::find_player());
		
	level_setup();

	GameObjects::update = true;
}

void Game::to_star_sector() {
	_level = _star_sector;
	
	Actor::set_buffer(_level->get_actors());
	Actor::set_map(_level->get_actor_map());

	_level->update();
	
	_player = static_cast<Player*>(GameObjects::find_player());

	level_setup();

	GameObjects::update = true;
}

void Game::to_solar_system() {
	_level = _solar_system;
	
	Actor::set_buffer(_level->get_actors());
	Actor::set_map(_level->get_actor_map());

	_level->update();
	
	_player = static_cast<Player*>(GameObjects::find_player());

	level_setup();

	GameObjects::update = true;
}

void Game::highlight_player() {
	for (Actor* a : _player->get_adjacent_actors_vec()) {
		_lightsystem.set_light(a, TCODColor(30, 30, 30));
	}
	GameObjects::update = true;
}

void Game::dehighlight_player() {
	for (Actor* a : _player->get_adjacent_actors_vec()) {
		_lightsystem.remove_light(a);
	}
	GameObjects::update = true;
}
