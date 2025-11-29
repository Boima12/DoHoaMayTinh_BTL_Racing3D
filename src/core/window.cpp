// Window and GLUT callbacks
#include "../../include/window.h"
#include "../../include/input.h"
#include "../../include/timer.h"
#include "../../include/game.h"
#include <GL/freeglut.h>
#include <iostream>

namespace {
    int g_width = 1280;
    int g_height = 720;
    std::string g_title = "Racing3D";
}

// Forward declarations for GLUT callbacks
static void display_cb();
static void reshape_cb(int w, int h);
static void keyboard_down_cb(unsigned char key, int x, int y);
static void keyboard_up_cb(unsigned char key, int x, int y);
static void special_down_cb(int key, int x, int y);
static void special_up_cb(int key, int x, int y);
static void mouse_cb(int button, int state, int x, int y);
static void timer_cb(int value);

namespace Window {
    void init(int argc, char** argv, int width, int height, const std::string &title) {
        g_width = width;
        g_height = height;
        g_title = title;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(g_width, g_height);
        glutCreateWindow(g_title.c_str());

        // basic GL state
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // sky-like

        // register callbacks
        glutDisplayFunc(display_cb);
        glutReshapeFunc(reshape_cb);
        glutKeyboardFunc(keyboard_down_cb);
        glutKeyboardUpFunc(keyboard_up_cb);
        glutSpecialFunc(special_down_cb);
        glutSpecialUpFunc(special_up_cb);
        glutMouseFunc(mouse_cb);

        // start timer loop
        glutTimerFunc(16, timer_cb, 0);
    }

    void startMainLoop() {
        glutMainLoop();
    }
}

static void display_cb() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Game::render();

    glutSwapBuffers();
}

static void reshape_cb(int w, int h) {
    g_width = w; g_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (h==0) ? 1.0f : (float)w / (float)h;
    gluPerspective(60.0, aspect, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

static void keyboard_down_cb(unsigned char key, int x, int y) {
    Input::onKeyDown(key, x, y);
}
static void keyboard_up_cb(unsigned char key, int x, int y) { Input::onKeyUp(key,x,y); }
static void special_down_cb(int key, int x, int y) { Input::onSpecialDown(key,x,y); }
static void special_up_cb(int key, int x, int y) { Input::onSpecialUp(key,x,y); }
static void mouse_cb(int button, int state, int x, int y) { Input::onMouse(button,state,x,y); }

static void timer_cb(int value) {
    float dt = Timer::delta();
    Game::update(dt);
    glutPostRedisplay();
    glutTimerFunc(16, timer_cb, 0);
}
