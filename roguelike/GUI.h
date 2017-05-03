#pragma once
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
	
	GUI(GUI_TYPE type, std::string text);

	~GUI();

	virtual void draw();

	static void set_buffer(std::vector<GUI*>* buffer) { _buffer = buffer; }
protected:
	int _x, _y, _width, _height;
	std::vector<Text> _text;
private:
	static std::vector<GUI*>* _buffer;
};

class Message_Box : public GUI {
public:
	Message_Box(std::string text);

	virtual void draw() override;
};