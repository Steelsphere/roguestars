#include "GameEvent.h"

GameEvent::GAME_EVENT GameEvent::_currevent = GameEvent::NONE;
GameEvent::GAME_EVENT GameEvent::_lastevent = GameEvent::NONE;
bool GameEvent::_locked = false;
int GameEvent::_param = 0;

GameEvent::GameEvent() {}
GameEvent::~GameEvent() {}
