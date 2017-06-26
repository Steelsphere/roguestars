#include "Input.h"
#include "GUI.h"
#include "GameEvent.h"
#include "GameObjects.h"

#include <memory>

Actor* Input::_reciever;
Input::MODE Input::_mode = NORMAL;
TCOD_key_t Input::_lastkey;


void Input::input(TCOD_key_t key) {
	if (key.pressed) {
		std::vector<Actor*> actors;
		switch (_mode) {

		case ESC:
			switch (key.vk) {
			case TCODK_ESCAPE:
				if (GameEvent::get_last_event() == GameEvent::NEW_ESC_MENU) {
					GameEvent::set_event(GameEvent::DELETE_ESC_MENU);
					_mode = NORMAL;
					break;
				}
				break;
			}
			break;
		
		case ENTER_TO_CONTINUE:
			switch (key.vk) {
			case TCODK_ENTER:
				_mode = NORMAL;
				break;
			}
			break;
			
		case CLOSE:
			switch (key.vk) {
			case TCODK_KP8:
				actors = Actor::get_actors(_reciever->get_world_pos()[0], _reciever->get_world_pos()[1] - 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP7:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] - 1, _reciever->get_world_pos()[1] - 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP9:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] + 1, _reciever->get_world_pos()[1] - 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP4:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] - 1, _reciever->get_world_pos()[1], 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP6:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] + 1, _reciever->get_world_pos()[1], 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP1:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] - 1, _reciever->get_world_pos()[1] + 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP2:
				actors = Actor::get_actors(_reciever->get_world_pos()[0], _reciever->get_world_pos()[1] + 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			case TCODK_KP3:
				actors = Actor::get_actors(_reciever->get_world_pos()[0] + 1, _reciever->get_world_pos()[1] + 1, 0);
				for (Actor* a : actors) {
					a->on_keypress_c();

				}
				_mode = NORMAL;
				break;
			}
			break;
		
		case INVENTORY_PANEL:
			switch (key.vk) {
			case TCODK_ESCAPE:
				GameEvent::unlock_event();
				GameEvent::set_event(GameEvent::CLOSE_INVENTORY);
				_mode = NORMAL;
				break;
			case TCODK_CHAR:
				switch (key.c) {
				case 'i':
					GameEvent::unlock_event();
					GameEvent::set_event(GameEvent::CLOSE_INVENTORY);
					_mode = NORMAL;
					break;
				}
			}
			break;

		case NORMAL:
			switch (key.vk) {
				// Movement
			case TCODK_KP8:
				_reciever->move("top");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP7:
				_reciever->move("topleft");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP9:
				_reciever->move("topright");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP4:
				_reciever->move("left");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP6:
				_reciever->move("right");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP1:
				_reciever->move("bottomleft");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP2:
				_reciever->move("bottom");
				GameObjects::new_turn = true;
				break;
			case TCODK_KP3:
				_reciever->move("bottomright");
				GameObjects::new_turn = true;
				break;

			case TCODK_TAB:
				if (GameEvent::get_last_event() == GameEvent::NEW_INFO_VIEWER) {
					GameEvent::unlock_event();
					GameEvent::set_event(GameEvent::DELETE_INFO_VIEWER);
					GameObjects::player_controlled = true;
					break;
				}
				GameEvent::set_event(GameEvent::NEW_INFO_VIEWER);
				GameEvent::lock_event();
				break;

			case TCODK_PAGEDOWN:
				if (GameObjects::player_controlled) {
					Actor::get_actor(_reciever->get_world_pos()[0], _reciever->get_world_pos()[1], 0)->on_pg_down();
				}
				break;

			case TCODK_PAGEUP:
				GameEvent::set_event(GameEvent::UPWARDS);
				break;

			case TCODK_HOME:
				GameEvent::set_event(GameEvent::ENTER_SPACESHIP);
				break;

			case TCODK_ESCAPE:
				_mode = ESC;
				GameEvent::set_event(GameEvent::NEW_ESC_MENU);
				break;

			case TCODK_CHAR:
				switch (key.c) {
				case 'c':
					_mode = CLOSE;
					break;
				case 'i':
					GameEvent::set_event(GameEvent::OPEN_INVENTORY);
					GameEvent::lock_event();
					_mode = INVENTORY_PANEL;
					break;
				}
				break;
			}
			break;
		}
		_lastkey = key;
	}
}

