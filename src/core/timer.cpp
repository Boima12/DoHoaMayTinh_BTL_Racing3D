/*
bộ đo thời gian rất nhỏ dùng GLUT để tính delta time (thời gian giữa hai frame), 
giá trị này được dùng để cập nhật vị trí, vận tốc, animation… trong game.
*/

// Simple timer using GLUT elapsed time
#include "../../include/timer.h"
#include <GL/freeglut.h>

namespace Timer {
    static int last = 0;
    void init() {
        last = glutGet(GLUT_ELAPSED_TIME);
    }
    float delta() {
        int now = glutGet(GLUT_ELAPSED_TIME);
        int diff = now - last;
        if (diff < 0) diff = 0;
        last = now;
        return diff / 1000.0f;
    }
}
