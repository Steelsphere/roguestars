#include "main.h"

// Non-platform independent file

#include <Windows.h>

int main() {

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	std::cout.rdbuf(NULL);
#endif

	Game game = Game();
	game.init();

	try {
		game.start();
	}
	catch (std::exception& e) {
		std::string error = std::string("An error has occured: ") + std::string(e.what());
		MessageBox(NULL, error.c_str(), NULL, MB_ICONERROR);
	}
	return 0;
}