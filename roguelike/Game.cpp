#include "Game.h"
#include "Camera.h"
#include "Input.h"
#include "GameEvent.h"
#include "AI.h"
#include "Character.h"
#include "Structure.h"
#include "Tile.h"
#include "Faction.h"

#include <filesystem>

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

	_time = Time(3000, 1, 1, 1, 1, 1);
	std::cout << _time.format_time("%M/%D/%Y %H:%m:%S") << std::endl;

	_time.pass_time(0, 24, 0, 0, 0, 0);
	std::cout << _time.format_time("%M/%D/%Y %H:%m:%S") << std::endl;

	GameObjects::log = _log;
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
	case GameEvent::SAVE_SCREEN:
		save_screen();
		break;
	case GameEvent::LOAD_LEVEL:
		load_level();
		break;
	case GameEvent::TEST_LEVEL:
		test_level();
		break;
	case GameEvent::OPEN_MAP:
		open_map();
		break;
	case GameEvent::CLOSE_MAP:
		close_map();
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
//		_level->update_tile(_player->get_world_pos()[0], _player->get_world_pos()[1], 0);
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
//		_level->update_tile(c->get_world_pos()[0], c->get_world_pos()[1], 0);
	}
}

void Game::startup_new_game() {
	destroy_main_menu();
	std::cout << "Size of one actor: " << sizeof(Actor) << std::endl;
	
	GameObjects::new_level_id = Random::random(Random::generator);

	_loadingscreen = new LoadingScreen("Generating the galaxy");
	_log = new Log(1, GameObjects::screen_height - 14, GameObjects::screen_width - 2, 6);
	
	GameObjects::log = _log;

	_loadingscreen->draw(true);
	_log->draw(true);

	new_galaxy();
	
	_gui_map = new Map(1, 1, GameObjects::screen_width - 2, GameObjects::screen_height - 15, _level, true);
	_gui_map->draw(true);
	TCODConsole::root->flush();

	generate_factions();
	
	delete _loadingscreen;
	delete _gui_map;
	delete _log;

	int num = 1;
	for (auto& f : std::experimental::filesystem::directory_iterator("Data\\Save")) {
		num++;
	}
	std::string gname = "Game " + std::to_string(num);
	std::experimental::filesystem::create_directory("Data\\Save\\" + gname);
	_savegame_directory = "Data\\Save\\" + gname;
	GameObjects::savegame_path = _savegame_directory;

	std::experimental::filesystem::create_directory(_savegame_directory + "\\galaxy");
	std::experimental::filesystem::create_directory(_savegame_directory + "\\starsector");
	std::experimental::filesystem::create_directory(_savegame_directory + "\\solarsystem");
	std::experimental::filesystem::create_directory(_savegame_directory + "\\world");
	std::experimental::filesystem::create_directory(_savegame_directory + "\\surface");

	_log = new Log;
	_status = new Status(_player, &_time);
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
	_status = new Status(_player, &_time);

}

void Game::new_main_menu() {
	delete _status;
	delete _log;
	delete _ESCMenu;
	_status = nullptr;
	_log = nullptr;
	_ESCMenu = nullptr;

	update();
	TCODConsole::root->flush();

	if (_level != nullptr) {
		std::string npath = "Data\\scr\\scr" + std::to_string(GameObjects::num_files_in_directory("Data\\scr") + 1);
		TCODConsole::root->saveApf(npath.c_str());
	}

	destroy_garbage();

	int choice = 0;
	int i = 0;
	if (GameObjects::num_files_in_directory("Data\\scr") != 0) {
		choice = Random::randc(1, GameObjects::num_files_in_directory("Data\\scr"));
		i = 1;
	}
	
	for (auto f : std::experimental::filesystem::directory_iterator("Data\\scr")) {
		if (i == choice) {
			TCODConsole* newcons = new TCODConsole(GameObjects::screen_width, GameObjects::screen_height);
			newcons->loadApf(f.path().string().c_str());
			
			TCODConsole::blit(newcons, 0, 0, 0, 0, TCODConsole::root, 0, 0);
			TCODConsole::root->flush();
			break;
		}
		i++;
	}

	_MainMenu = new MainMenu;
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
	save_level();
	
	loading_screen();
	
	delete _level;
	_world = new World(1024);
	_world->generate_world();
	
	_level = _world->get_current_level();
	_level->save_level_image("Data\\Level.png");
	_level->set_savedir("surface");
	_world_id = _level->id;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	_player->add_to_inventory(new Item(0, 0, 0, Item::DIGITAL_WATCH));
	
	level_setup();
	_lightsystem.set_global_lighting(_level, 15, 15, 0);
	
	GameObjects::update = true;
}

void Game::new_solar_system() {
	save_level();
	
	delete _level;
	_level = new Level;
	_level->generate_level(256, Level::SOLAR_SYSTEM);
	_level->set_savedir("solarsystem");
	_solarsystem_id = _level->id;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player(Level::SOLAR_SYSTEM);
	
	level_setup();

	GameObjects::update = true;
}

void Game::new_star_sector() {
	save_level();
	
	delete _level;
	_level = new Level;
	_level->generate_level(512, Level::STAR_SECTOR);
	_level->set_savedir("starsector");
	_starsector_id - _level->id;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player(Level::STAR_SECTOR);
	
	level_setup();
	GameObjects::update = true;
}

void Game::new_galaxy() {
	save_level();
	
	delete _level;
	_level = new Level;
	_level->generate_level(1024, Level::GALAXY);
	_level->set_savedir("galaxy");
	_galaxy_id = _level->id;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player(Level::GALAXY);

	level_setup();
	GameObjects::update = true;
}

void Game::new_world_map() {
	if (_level->get_type() == Level::WORLD_MAP) {
		return;
	}
	
	loading_screen();
	save_level();

	_lightsystem.clear_cache();
	if (_world != nullptr && _level != nullptr) {
		delete _level;
		_player = nullptr;

		_level = _world->generate_world_map();
		_level->set_savedir("world");

		_player = new Player(_world->get_current_pos().first, _world->get_current_pos().second, 0, '@', TCODColor::blue);
		
		level_setup();
		_camera->update();
	}
	
	GameObjects::update = true;
}

void Game::enter_world_tile() {
	loading_screen();
	save_level();

	delete _level;
	_world->new_level(_player->get_world_pos()[0], _player->get_world_pos()[1]);
	_level = _world->get_current_level();
	_level->set_savedir("surface");
	_level->id = GameObjects::new_level_id;

	_player = new Player(0, 0, 0, '@', TCODColor::blue);
	_player->spawn_player_in_world();
	
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
	if (_level->get_type() == Level::GALAXY || Level::NONE) {
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
	else if (_level->get_type() > Level::WORLD_MAP) {
		to_world_map();
	}
}

void Game::to_galaxy() {
	
	if (GameObjects::is_directory_empty(_savegame_directory + "\\galaxy")) {
		new_galaxy();
		fix_tile_id<StarSector>();
		return;
	}
	
	GameObjects::level_id_to_load = _galaxy_id;
	
	load_level();
	_level->set_savedir("galaxy");
	_level->update();

	_player = GameObjects::find_player(_level);
	level_setup();

	fix_tile_id<StarSector>();
	GameObjects::update = true;
}

void Game::to_star_sector() {
	
	if (GameObjects::is_directory_empty(_savegame_directory + "\\starsector")) {
		new_star_sector();
		fix_tile_id<SolarSystem>();
		return;
	}
	
	GameObjects::level_id_to_load = _starsector_id;
	
	load_level();
	_level->set_savedir("starsector");
	_level->update();

	_player = GameObjects::find_player(_level);
	level_setup();

	fix_tile_id<SolarSystem>();
	GameObjects::update = true;
}

void Game::to_solar_system() {
	
	if (GameObjects::is_directory_empty(_savegame_directory + "\\solarsystem")) {
		new_solar_system();
		fix_tile_id<Planet>();
		return;
	}
	
	GameObjects::level_id_to_load = _solarsystem_id;
	
	load_level();
	_level->set_savedir("solarsystem");
	_level->update();
	
	_player = GameObjects::find_player(_level);
	level_setup();

	fix_tile_id<Planet>();
	GameObjects::update = true;
}

void Game::to_world_map() {
	
	if (GameObjects::is_directory_empty(_savegame_directory + "\\world")) {
		new_world_map();
		fix_tile_id<Biome>();
		return;
	}

	GameObjects::level_id_to_load = _world_id;
	
	load_level();
	_level->set_savedir("world");
	_level->update();

	_player = GameObjects::find_player(_level);
	level_setup();

	fix_tile_id<Biome>();
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

void Game::save_screen() {
	_MainMenu->save_screen();
}

void Game::load_level() {
	if (GameObjects::level_id_to_load == 0) {
		return;
	}
	
	for (auto f : std::experimental::filesystem::recursive_directory_iterator(_savegame_directory)) {
		std::string fname = f.path().filename().string();
		
		std::cout << fname << std::endl;

		if (std::string(fname) == std::to_string(GameObjects::level_id_to_load)) {
			delete _level;
				
			_level = Level::load_level_file(f.path().string());
			_level->id = GameObjects::level_id_to_load;
			_player = GameObjects::find_player(_level);

			level_setup();

			_log = new Log;
			_status = new Status(_player, &_time);
			
			std::cout << "Loaded level with ID: " << GameObjects::level_id_to_load << std::endl;
			
			GameObjects::level_id_to_load = 0;
			GameObjects::update = true;
			return;
		}
	}
	GameObjects::level_id_to_load = 0;
}

void Game::save_level() {
	if (_level == nullptr) {
		return;
	}
	
	std::string name = std::to_string(_level->id);
	name.insert(0, _level->get_savedir() + "\\");
	_level->save_level_file(_savegame_directory + "\\" + name);

	std::cout << "Saved level with ID: " << name << std::endl;
}

template<typename T>
void Game::fix_tile_id() {
	for (Actor* a : Actor::get_actors(_player->get_world_pos()[0], _player->get_world_pos()[1], 0)) {
		if (typeid(*a).name() == typeid(T).name()) {
			static_cast<T*>(a)->id = GameObjects::old_level_id;
			std::cout << "Tile ID fixed successfully to " << static_cast<T*>(a)->id << std::endl;
		}
	}
}

void Game::test_level() {
	destroy_main_menu();
	TCODConsole::root->clear();

	_level = new Level;
	_level->generate_level(128, Level::TEST);
	
	_player = new Player(32, 32, 0, '@', TCODColor::blue);
	level_setup();

	_log = new Log;
	_status = new Status(_player, &_time);

	GameObjects::update = true;
}

void Game::open_map() {
	_gui_map = new Map(_level);
}

void Game::close_map() {
	delete _gui_map;
	_gui_map = nullptr;
}

void Game::generate_factions() {
	
	
	// Find suitable spawn points
	std::vector<Actor*> spawnpoints;
	
	for (Actor* a : (*_level->get_actors())) {
		if (a->get_name() == "Star Sector") {
			spawnpoints.push_back(a);
		}
	}
	
	for (int i = 0; i < 5; i++) {
		int r = Random::randc(0, spawnpoints.size() - 1);
		new Faction(spawnpoints[r]->get_world_pos()[0], spawnpoints[r]->get_world_pos()[1]);
		spawnpoints.erase(spawnpoints.begin() + r);
		if (spawnpoints.size() == 0) {
			break;
		}
	}

	// Simulate
	std::cout << "Beginning faction simulation!\n";
	Faction::save_faction_map("Data\\anim\\0.png", _level->get_size());

	for (int i = 0; i < 10000; i++) {
		for (Faction* f : Faction::get_factions()) {
			f->simulate();
			std::cout << i << "/" << 10000 << "\r";
			
			if (i % 100 == 0) {
				_loadingscreen->set_text("Simulating the galaxy, turns simulated:" + std::to_string(i) + "/" + std::to_string(10000));
				TCODConsole::root->flush();
			}

			if (i % 1000 == 0) {
				for (Faction* f : Faction::get_factions()) {
					for (Actor* a : f->get_owned_tiles()) {
						a->set_bcolor_obj(f->get_color());
					}
				}
				_gui_map->update_map(_level, true);
				_gui_map->draw(true);
				TCODConsole::root->flush();
			}
			
			if (Random::randc(0, 4000) == 1) {
				int r = Random::randc(0, spawnpoints.size() - 1);
				new Faction(spawnpoints[r]->get_world_pos()[0], spawnpoints[r]->get_world_pos()[1]);
				
				std::cout << "A new nation has been founded\n";
				_log->message("A new nation has been founded", TCODColor::green);
				_log->draw(true);

				spawnpoints.erase(spawnpoints.begin() + r);
				if (spawnpoints.size() == 0) {
					break;
				}
			}

//			if (i % 1000 == 0) {
//				Faction::save_faction_map("Data\\anim\\" + std::to_string(i) + ".png", _level->get_size());
//			}
		}
	}

	// Set color of tiles on the level
	for (Faction* f : Faction::get_factions()) {
		for (Actor* a : f->get_owned_tiles()) {
			a->set_bcolor_obj(f->get_color());
			
			// Change color of star sectors based on how dangerous it is
			if (a->get_name() == "Star Sector") {
				int dangerlevel = 0;
				for (Actor* ab : a->get_adjacent_actors_vec()) {
					if (f->other_own_tile(ab)) {
						dangerlevel++;
					}
				}
				if (dangerlevel != 0) {
					a->set_color(255, 255 - (30 * dangerlevel), 255 - (30 * dangerlevel));
				}
			}
		}
		// Capital color
		f->get_capital()->set_color_obj(TCODColor::yellow);
	}

	// Save factions image
	Faction::save_faction_map("Data\\factionmap.png", _level->get_size());
}