#include "main.h"

#include <string>

// Non-platform independent file

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	std::cout.rdbuf(NULL);
#endif

	game.init();

#ifdef _WIN32
	try {
#endif
		game.start();
#ifdef _WIN32
	}
	catch (std::exception& e) {
		std::string error = std::string("An error has occured: ") + std::string(e.what());
		MessageBox(NULL, error.c_str(), NULL, MB_ICONERROR);
	}
#endif
	return 0;
}