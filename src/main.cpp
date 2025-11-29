#include "../include/window.h"
#include "../include/input.h"
#include "../include/timer.h"
#include "../include/game.h"

int main(int argc, char** argv) {
	// initialize subsystems
	Input::init();
	Timer::init();
	Game::init();

	// create window and start main loop
	Window::init(argc, argv);
	Window::startMainLoop();

	return 0;
}
