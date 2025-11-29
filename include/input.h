#ifndef INPUT_H
#define INPUT_H

#include <utility>

namespace Input {
	void init();
	void onKeyDown(unsigned char key, int x, int y);
	void onKeyUp(unsigned char key, int x, int y);
	void onSpecialDown(int key, int x, int y);
	void onSpecialUp(int key, int x, int y);
	void onMouse(int button, int state, int x, int y);

	// returns accumulated wheel delta since last call (positive = wheel up)
	int wheelDelta();

	bool isKeyDown(unsigned char key);
	bool isSpecialDown(int key);
	std::pair<int,int> mousePos();
}

#endif // INPUT_H

