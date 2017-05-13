#pragma once
class GameEvent {
public:
	enum GAME_EVENT {
		NONE,
		STARTUP_NEW_GAME,
		STARTUP_LOAD_GAME,
		EXIT,
	};
	
	GameEvent();
	~GameEvent();

	static GAME_EVENT get_event() { return _currevent; }
	
	static void set_event(GAME_EVENT ev) { _currevent = ev; }

	static void reset_event() { _currevent = NONE; }

private:
	static GAME_EVENT _currevent;
};

