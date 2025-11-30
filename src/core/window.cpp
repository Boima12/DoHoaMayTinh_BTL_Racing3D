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
        // glClearColor(0.91f, 0.58f, 0.38f, 1.0f); // warm sunset sky

        // Lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        // warmer light tones (slightly yellow/orange)
        GLfloat lightAmbient[]  = {0.24f, 0.18f, 0.12f, 1.0f};
        GLfloat lightDiffuse[]  = {1.00f, 0.90f, 0.70f, 1.0f};
        GLfloat lightSpecular[] = {1.00f, 0.95f, 0.80f, 1.0f};
        GLfloat lightPos[]      = {50.0f, 100.0f, 50.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Use glColor to set material ambient/diffuse; set specular via glMaterial
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        GLfloat globalSpec[] = {0.2f,0.2f,0.2f,1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalSpec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 16.0f);

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
