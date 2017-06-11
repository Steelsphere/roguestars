#include "Structure.h"
#include "Level.h"
#include "Tile.h"

Structure::Structure(int x, int y, int w, int h, S_TYPE type, Tile::TILE_TYPE wall, Tile::TILE_TYPE floor) {
	std::string s;
	switch (type) {
	case CABIN:
		s =
			"#########\n"
			"#.......#\n"
			"#.......#\n"
			"#.......#\n"
			"........#\n"
			"#.......#\n"
			"#.......#\n"
			"#.......#\n"
			"#########\n";
		w = 9;
		h = 9;
		break;
	}
	int xs = 0;
	int ys = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '\n') {
			ys++;
			xs = 0;
		}
		else {
			if (s[i] == '#') {
				new Tile(x + xs, y + ys, 0, wall);
			}
			if (s[i] == '.') {
				new Tile(x + xs, y + ys, 0, floor);
			}
			xs++;
		}
	}
	Structure::~Structure();
}
Structure::~Structure(){}
