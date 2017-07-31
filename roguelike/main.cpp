#include "main.h"

int main() {
	
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	std::cout.rdbuf(NULL);
#endif
	
	Game game = Game();
	game.init();
	game.start();
	return 0;
}