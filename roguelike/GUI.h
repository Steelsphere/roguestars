#pragma once

#include "GameEvent.h"

#include <string>
#include <libtcod\libtcod.hpp>
#include <vector>

class GUI
{
public:
	enum GUI_TYPE {
		MESSAGE_BOX,
	};
	
	struct Text {
		int x, y, w, h;
		std::string str;
		TCODColor color;
	};
	
	GUI(int x, int y, int w, int h, std::vector<Text> text);
	
	~GUI();

	virtual void draw();

	static std::vector<GUI*>* get_buffer() { return &_buffer; }

	void position_text();
protected:
	int _x, _y, _width, _height;
	std::vector<Text> _text;
private:
	static std::vector<GUI*> _buffer;
};

class Message_Box : public GUI {
public:
	Message_Box(std::string text);

	virtual void draw() override;
};

class Log : public GUI {
public:
	Log();

	void message(std::string message, TCODColor color);
};

class Status : public GUI {
public:
	Status();
};

class Main_Menu : public GUI {
public:
	struct MText {
		int x, y, w, h;
		std::string str;
		TCODColor color;
		bool selected;
		GameEvent::GAME_EVENT action;
	};
	
	enum STATE {
		FRONT,
	};
	
	Main_Menu();

	void front();

	virtual void draw() override;

	void set_selector();
private:
	STATE _state = FRONT;
	std::vector<MText> _mtext;
};
