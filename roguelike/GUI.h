#pragma once

#include "GameEvent.h"
#include "Actor.h"

#include <string>
#include <libtcod\libtcod.hpp>
#include <vector>

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
	};
	
	GUI();
	
	GUI(int x, int y, int w, int h, std::vector<Text> text);
	
	~GUI();

	virtual void draw(bool force = false);

	static std::vector<GUI*>* get_buffer() { return &_buffer; }

	virtual void position_text();
protected:
	int _x, _y, _width, _height;
	float _transparency;
	bool _update;
	std::vector<Text> _text;
	TCODConsole* _cons;
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

	void message(const std::string& message, TCODColor color);
};

class Status : public GUI {
public:
	Status();
};

class SelectionBox : public GUI {
public:
	struct MText {
		int x, y, w, h;
		std::string str;
		TCODColor color;
		bool selected;
		GameEvent::GAME_EVENT action;
	};
	
	SelectionBox();

	SelectionBox(int x, int y, int w, int h, std::vector<Text> text);

	virtual void draw(bool force = false) override;

	virtual void position_text() override;

	void set_selector();
protected:
	std::vector<MText> _mtext;
};

class MainMenu : public SelectionBox {
public:
	enum STATE {
		FRONT,
	};
	
	MainMenu();

	void front();

	virtual void draw(bool force = false) override;

private:
	STATE _state = FRONT;
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