#include "../../include/game.h"
#include "../../include/camera.h"
#include <GL/freeglut.h>
#include <cmath>

namespace Game {
	static float angle = 0.0f;
	static Camera g_camera;
	void init() {
		angle = 0.0f;
		g_camera = Camera();
		g_camera.setDistance(6.0f);
	}
	void update(float dt) {
		// simple animation so we can see something
		angle += 30.0f * dt; // degrees per second
		if (angle > 360.0f) angle -= 360.0f;

		// target is origin for now
		Vec3 target{0.0f, 0.0f, 0.0f};
		g_camera.update(target, dt);
	}
	void render() {
		glLoadIdentity();
		g_camera.applyView();

		// draw ground
		glDisable(GL_LIGHTING);
		glColor3f(0.3f, 0.8f, 0.3f);
		glBegin(GL_QUADS);
			glVertex3f(-50.0f, -0.01f, -50.0f);
			glVertex3f( 50.0f, -0.01f, -50.0f);
			glVertex3f( 50.0f, -0.01f,  50.0f);
			glVertex3f(-50.0f, -0.01f,  50.0f);
		glEnd();

		// draw a rotating cube as placeholder car
		glPushMatrix();
			glTranslatef(0.0f, 0.5f, 0.0f);
			glRotatef(angle, 0.0f, 1.0f, 0.0f);
			glColor3f(0.8f, 0.1f, 0.1f);
			glutSolidCube(1.0);
		glPopMatrix();
	}
}
