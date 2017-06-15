#include "Structure.h"
#include "Level.h"
#include "Tile.h"

Structure::Structure(int x, int y, S_TYPE type) {
	std::string s;
	int w = 0;
	int h = 0;
	
	switch (type) {
	case CABIN:
		s =
			"WWWWWWWWW\n"
			"WdddddddW\n"
			"WdddddddW\n"
			"WdddddddW\n"
			"ddddddddW\n"
			"WdddddddW\n"
			"WdddddddW\n"
			"WdddddddW\n"
			"WWWWWWWWW\n";
		w = 9;
		h = 9;
		break;
	case TINY_SPACESHIP:
		s =
			"SGGGS\n"
			"SsssS\n"
			"SsssS\n"
			"SsssS\n"
			"SSsSS\n";
		w = 5;
		h = 4;
	}
	int xs = 0;
	int ys = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '\n') {
			ys++;
			xs = 0;
		}
		else {
			switch (s[i]) {
			case 'W':
				new Tile(x + xs, y + ys, 0, Tile::WOOD);
				break;
			case 'd':
				new Tile(x + xs, y + ys, 0, Tile::DIRT);
				break;
			case 'G':
				new Tile(x + xs, y + ys, 0, Tile::GLASS);
				break;
			case 'S':
				new Tile(x + xs, y + ys, 0, Tile::STEEL_WALL);
				break;
			case 's':
				new Tile(x + xs, y + ys, 0, Tile::STEEL_FLOOR);
				break;
			}
			xs++;
		}
	}
	Structure::~Structure();
}
Structure::~Structure(){}
