#include "../../include/game.h"
#include "../../include/camera.h"
#include "../../include/car.h"
#include "../../include/track.h"
#include "../../include/scenery.h"
#include "../../include/config.h"
#include <GL/freeglut.h>
#include <cmath>

// define config defaults (global scope)
bool Config::showScenery = true;

namespace Game {
	static float angle = 0.0f;
	static Camera g_camera;
	static Car g_car;
	static Track g_track;
	static Scenery g_scenery;
	void init() {
		angle = 0.0f;
		g_camera = Camera();
		g_camera.setDistance(6.0f);
		g_car.reset();

		g_track = Track();
		if (!g_track.loadFromFile("track_points.txt")) {
			g_track.initDefault();   // fallback nếu file lỗi
		}
		g_scenery.initWithTrack(&g_track);
	}
	void update(float dt) {
		// simple animation so we can see something
		angle += 30.0f * dt; // degrees per second
		if (angle > 360.0f) angle -= 360.0f;

		// update car and camera target
		g_car.update(dt);
		Vec3 target = g_car.position;
		target.y = 0.0f;
		g_camera.update(target, dt);
	}
	void render() {
		glLoadIdentity();
		g_camera.applyView();

		// draw ground
		glDisable(GL_LIGHTING);
		glColor3f(0.3f, 0.8f, 0.3f);
		glBegin(GL_QUADS);
			glVertex3f(-150.0f, -0.01f, -150.0f);
			glVertex3f( 150.0f, -0.01f, -150.0f);
			glVertex3f( 150.0f, -0.01f,  150.0f);
			glVertex3f(-150.0f, -0.01f,  150.0f);
		glEnd();

		// draw the car
		if (Config::showScenery) g_scenery.draw();
		g_track.draw();
		g_car.draw();
	}
}
