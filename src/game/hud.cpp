// Simple HUD rendering using bitmap fonts
#include "../../include/hud.h"
#include "../../include/config.h"
#include <GL/freeglut.h>
#include <cstdio>

namespace HUD {

void drawHUD(const Car &car, int laps, float lapTime, float totalTime) {
    int ww = glutGet(GLUT_WINDOW_WIDTH);
    int hh = glutGet(GLUT_WINDOW_HEIGHT);

    // switch to orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, ww, 0, hh);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    char buf[128];
    std::snprintf(buf, sizeof(buf), "Speed: %.1f", car.speed);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, hh - 20);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "Laps: %d", laps);
    glRasterPos2i(10, hh - 40);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    int ltMin = (int)(lapTime / 60.0f);
    float ltSec = lapTime - ltMin * 60;
    std::snprintf(buf, sizeof(buf), "Lap Time: %02d:%04.1f", ltMin, ltSec);
    glRasterPos2i(10, hh - 60);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    int ttMin = (int)(totalTime / 60.0f);
    float ttSec = totalTime - ttMin * 60;
    std::snprintf(buf, sizeof(buf), "Total Time: %02d:%04.1f", ttMin, ttSec);
    glRasterPos2i(10, hh - 80);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Controls hints (expanded)
    std::snprintf(buf, sizeof(buf), "Middle Mouse - Zoom");
    glRasterPos2i(10, hh - 110);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "WASD - Move Car");
    glRasterPos2i(10, hh - 130);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "Arrow Keys - Move Camera");
    glRasterPos2i(10, hh - 150);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "C - Toggle Scenery");
    glRasterPos2i(10, hh - 170);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "V - Toggle Walls");
    glRasterPos2i(10, hh - 190);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    std::snprintf(buf, sizeof(buf), "SPACE - Reset All");
    glRasterPos2i(10, hh - 210);
    for (const char *c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // re-enable lighting
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

} // namespace HUD
