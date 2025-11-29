// Basic input state tracking
#include "../../include/input.h"
#include <cstring>
#include <utility>

namespace Input {
    static bool keys[256];
    static bool special[256];
    static int mouse_x = 0, mouse_y = 0;
    static int wheel_accum = 0;

    void init() {
        std::memset(keys, 0, sizeof(keys));
        std::memset(special, 0, sizeof(special));
    }

    void onKeyDown(unsigned char key, int x, int y) {
        keys[key] = true;
        if (key == 27) { // ESC
            exit(0);
        }
    }
    void onKeyUp(unsigned char key, int x, int y) {
        keys[key] = false;
    }
    void onSpecialDown(int key, int x, int y) {
        if (key >= 0 && key < 256) special[key] = true;
    }
    void onSpecialUp(int key, int x, int y) {
        if (key >= 0 && key < 256) special[key] = false;
    }
    void onMouse(int button, int state, int x, int y) {
        mouse_x = x;
        mouse_y = y;
        // wheel events handled as special buttons in freeglut (BUTTON4/BUTTON5)
        if (button == 3) {
            // wheel up
            wheel_accum += 1;
        } else if (button == 4) {
            // wheel down
            wheel_accum -= 1;
        }
    }

    bool isKeyDown(unsigned char key) { return keys[key]; }
    bool isSpecialDown(int key) { return (key>=0 && key<256) ? special[key] : false; }
    std::pair<int,int> mousePos() { return {mouse_x, mouse_y}; }

    int wheelDelta() { int v = wheel_accum; wheel_accum = 0; return v; }
}
