#pragma once
class GameEvent {
public:
	enum GAME_EVENT {
		NONE,
		TO_MAIN_MENU,
		STARTUP_NEW_GAME,
		STARTUP_LOAD_GAME,
		NEW_ESC_MENU,
		DELETE_ESC_MENU,
		EXIT,
	};
	
	GameEvent();
	~GameEvent();

	static GAME_EVENT get_event() { return _currevent; }
	
	static GAME_EVENT get_last_event() { return _lastevent; }

	static void set_event(GAME_EVENT ev) { _currevent = ev; _lastevent = ev; }

	static void reset_event() { _currevent = NONE; }

private:
	static GAME_EVENT _currevent;
	static GAME_EVENT _lastevent;
};

