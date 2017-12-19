#pragma once
#define TCOD_SDL2

#include "GameEvent.h"
#include "Character.h"
#include "Time.h"

#include <string>
#include <libtcod.hpp>
#include <vector>

class Player;
class Level;
class StarSector;

class GUI
{
public:
	enum GUI_TYPE {
		NO_BACKGROUND,
		FILLED_BACKGROUND,
		FILLED_BORDERED_BACKGROUND,
	};
	
	struct Text {
		int x, y, w, h;
		std::string str;
		TCODColor color;
		bool ovrcolor = false;
		TCODColor bcolor = TCODColor::black;
	};
	
	GUI();
	
	GUI(int x, int y, int w, int h, std::vector<Text> text = std::vector<Text>());
	
	~GUI();

	virtual void draw(bool force = false);

	static std::vector<GUI*>* get_buffer() { return &_buffer; }

	// Some helper functions
	virtual void position_text();
	void make_transparency_work();
	void clear_working_area();

protected:
	int _x, _y, _width, _height;
	float _transparency;
	bool _update;
	std::vector<Text> _text;
	TCODConsole* _cons;
	TCODConsole* _screencons = nullptr;
	GUI_TYPE _type;
private:
	static std::vector<GUI*> _buffer;
};

class Message_Box : public GUI {
public:
	Message_Box(std::string text, bool nocontinue = false);

	virtual void draw(bool force = false) override;
};

class Log : public GUI {
public:
	Log();
	Log(int x, int y, int w, int h);

	void message(const std::string& message, TCODColor color);
};

class HealthInfo : public GUI {
public:
	HealthInfo(int x, int y, int w, int h, Player* p);
};

class TileInfo : public GUI {
public:
	TileInfo(int x, int y, int w, int h, Player* p);

	void update();

private:
	Player* _player;
};

class CharInfo : public GUI {
public:
	CharInfo(int x, int y, int w, int h, Player* p);
};

class MiscInfo : public GUI {
public:
	MiscInfo(int x, int y, int w, int h, Player* p);
};

class SectorInfo : public GUI {
public:
	SectorInfo(int x, int y, int w, int h, StarSector* s);

	void update();

	virtual void draw(bool force = false) override;

	StarSector* get_sector() { return _sector; }

private:
	StarSector* _sector;
};

class Status : public GUI {
public:
	Status(Player* player, Time* time);

	~Status();

	virtual void draw(bool force = false) override;

private:
	Player* _player;
	Time* _time;
	
	HealthInfo* _health;
	TileInfo* _tile;
	CharInfo* _char;
	MiscInfo* _misc;
	SectorInfo* _si = nullptr;
};

class SelectionBox : public GUI {
public:
	struct MText {
		int x, y, w, h;
		std::string str;
		TCODColor color;
		bool selected;
		GameEvent::GAME_EVENT action;
		TCODColor bcolor = TCODColor::black;
	};
	
	SelectionBox();

	SelectionBox(int x, int y, int w, int h, std::vector<Text> text);

	virtual void draw(bool force = false) override;

	virtual void position_text() override;

	void set_selector(bool force = false);
protected:
	std::vector<MText> _mtext;
};

class SelectionBoxEx : public SelectionBox {
public:
	const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	SelectionBoxEx(int x, int y, int w, int h);

	void update_mtext();
};

class Picture : public GUI {
public:
	Picture(Text t);
};

class MainMenu : public SelectionBox {
public:
	enum STATE {
		FRONT,
	};
	
	MainMenu();
	~MainMenu();

	void front();

	void save_screen();

	virtual void draw(bool force = false) override;

private:
	STATE _state = FRONT;
	Picture* _title_pic;
	Picture* _star_pic;
};

class ESCMenu : public SelectionBox {
public:
	ESCMenu();
};

class InfoViewer : public GUI {
public:
	InfoViewer(Actor* aref);

	virtual void draw(bool force = false) override;

protected:
	Actor* _actor;
};

class InventoryPanel : public SelectionBoxEx {
public:
	InventoryPanel(Character* c);
};

class Map : public GUI {
public:
	Map(Level* level, bool background = false, Player* player = nullptr);
	Map(int x, int y, int w, int h, Level* level, bool background);
	
	~Map();

	void update_map(Level* level, bool background, Player* player = nullptr);
};

class LoadingScreen : public GUI {
public:
	LoadingScreen(const std::string& text);

	void set_text(const std::string& text);
};

class TextBox : public GUI {
public:
	TextBox(int x, int y, int w, int h, std::string title, std::string descr, bool digits_only = false);

	virtual void draw(bool force = false) override;

	std::string get_value() { return _val; }

	bool is_done() { return _done; }

private:
	std::string _val = "";
	bool _done = false;
	bool _digits_only = false;
};