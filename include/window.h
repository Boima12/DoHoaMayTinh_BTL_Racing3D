#ifndef WINDOW_H
#define WINDOW_H

#include <string>

namespace Window {
	void init(int argc, char** argv, int width = 1280, int height = 720, const std::string &title = "Racing3D");
	void startMainLoop();
}

#endif // WINDOW_H

