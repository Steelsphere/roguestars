#include "Input.h"
#include "GUI.h"
#include "GameEvent.h"

#include <memory>

Actor* Input::_reciever;
Input::MODE Input::_mode = NORMAL;
TCOD_key_t Input::_lastkey;

void Input::input(TCOD_key_t key) {
	if (key.pressed) {
		switch (_mode) {

		case NORMAL:
			switch (key.vk) {

				// Movement
			case TCODK_KP8:
				_reciever->move("top");
				break;
			case TCODK_KP7:
				_reciever->move("topleft");
				break;
			case TCODK_KP9:
				_reciever->move("topright");
				break;
			case TCODK_KP4:
				_reciever->move("left");
				break;
			case TCODK_KP6:
				_reciever->move("right");
				break;
			case TCODK_KP1:
				_reciever->move("bottomleft");
				break;
			case TCODK_KP2:
				_reciever->move("bottom");
				break;
			case TCODK_KP3:
				_reciever->move("bottomright");
				break;

			case TCODK_TAB:
				if (GameEvent::get_last_event() == GameEvent::NEW_INFO_VIEWER) {
					GameEvent::unlock_event();
					GameEvent::set_event(GameEvent::DELETE_INFO_VIEWER);
					break;
				}
				GameEvent::set_event(GameEvent::NEW_INFO_VIEWER);
				GameEvent::lock_event();
				break;
			}

		case ESC:
			switch (key.vk) {
			case TCODK_ESCAPE:
				if (GameEvent::get_last_event() == GameEvent::NEW_ESC_MENU) {
					GameEvent::set_event(GameEvent::DELETE_ESC_MENU);
					_mode = NORMAL;
					break;
				}
				GameEvent::set_event(GameEvent::NEW_ESC_MENU);
				break;
			}
		case ENTER_TO_CONTINUE:
			switch (key.vk) {
			case TCODK_ENTER:
				_mode = NORMAL;
				break;
			}
			break;
		}
		_lastkey = key;
	}
}
