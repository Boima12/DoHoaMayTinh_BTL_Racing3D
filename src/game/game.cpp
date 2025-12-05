#include "../../include/game.h"
#include "../../include/camera.h"
#include "../../include/car.h"
#include "../../include/track.h"
#include "../../include/scenery.h"
#include "../../include/hud.h"
#include "../../include/config.h"
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

// define config defaults (global scope)
bool Config::showScenery = true;
bool Config::showWalls = true;
int Config::N_tree = 8;

namespace Game {
	static float angle = 0.0f;
	static Camera g_camera;
	static Car g_car;
	static Track g_track;
	static Scenery g_scenery;
	// HUD / timing
	static float g_totalTime = 0.0f;
	static float g_lapTime = 0.0f;
	static int g_laps = 0;
	static float g_lastCrossTime = -9999.0f;
    static Vec3 g_prevCarPos{0,0,0};
	void init() {
		angle = 0.0f;
		g_camera = Camera();
		g_camera.setDistance(6.0f);
		g_car.reset();

		g_track = Track();
		if (!g_track.loadFromFile("track_points.txt")) {
			g_track.initDefault();   // fallback nếu file lỗi
		}
		// Place car at start of track (sampled[0]) and align yaw to track forward
		if (!g_track.sampled.empty()) {
			const Vec3 &s0 = g_track.sampled[0];
			const Vec3 &s1 = g_track.sampled[(1) % g_track.sampled.size()];
			// compute yaw so that sin(yaw)=forward.x and cos(yaw)=forward.z
			float fx = s1.x - s0.x;
			float fz = s1.z - s0.z;
			float flen = sqrtf(fx*fx + fz*fz); if (flen == 0.0f) flen = 1.0f;
			fx /= flen; fz /= flen;
			float yawRad = atan2f(fx, fz); // yaw where sin(yaw)=fx, cos(yaw)=fz
			g_car.yaw = yawRad * 180.0f / 3.14159265f;
			// set position: back up 2.5 units from the start line so car is behind the line
			g_car.position.x = s0.x - fx * 2.5f;
			g_car.position.y = s0.y;
			g_car.position.z = s0.z - fz * 2.5f;

			// Place initial camera behind the car and looking down from above
			// compute a camera world position a few units behind and above the car
			Vec3 camPos;
			float camDist = g_camera.distance();
			camPos.x = g_car.position.x - fx * camDist;
			camPos.z = g_car.position.z - fz * camDist;
			camPos.y = g_car.position.y + 3.0f; // elevated for top-down view
			Vec3 camTarget = g_car.position;
			camTarget.y = g_car.position.y; // look at car's ground level
			// snap camera immediately to this position so it doesn't start in front or underground
			g_camera.snapTo(camPos, camTarget);
		}
		g_scenery.initWithTrack(&g_track);
	}
	void update(float dt) {
		// simple animation so we can see something
		angle += 30.0f * dt; // degrees per second
		if (angle > 360.0f) angle -= 360.0f;

		// update timers
		g_totalTime += dt;
		g_lapTime += dt;

		// handle one-shot SPACE reset (edge detect)
		static bool prevSpace = false;
		bool curSpace = Input::isKeyDown(' ');
		if (curSpace && !prevSpace) {
			// reset car to track start and clear timers/laps
			g_car.resetToTrack(&g_track, 1.5f);
			g_totalTime = 0.0f;
			g_lapTime = 0.0f;
			g_laps = 0;
			g_lastCrossTime = -9999.0f;
		}
		prevSpace = curSpace;

		// update car and camera target
		g_car.update(dt, &g_track);
		Vec3 target = g_car.position;
		target.y = 0.0f;
		g_camera.update(target, dt);

		// Improved lap detection: detect crossing of the start line (perpendicular to first segment)
		if (!g_track.sampled.empty()) {
			const Vec3 &s0 = g_track.sampled[0];
			const Vec3 &s1 = g_track.sampled[(1) % g_track.sampled.size()];
			// forward vector along track at start
			float fx = s1.x - s0.x; float fz = s1.z - s0.z;
			float flen = sqrtf(fx*fx + fz*fz); if (flen == 0.0f) flen = 1.0f; fx /= flen; fz /= flen;
			// normal pointing across the start line (to the left)
			float nx = -fz; float nz = fx;
			// prev and curr positions
			Vec3 prev = g_prevCarPos;
			Vec3 curr = g_car.position;
			// compute signed distances to the start line (using normal)
			float dPrev = (prev.x - s0.x) * nx + (prev.z - s0.z) * nz;
			float dCurr = (curr.x - s0.x) * nx + (curr.z - s0.z) * nz;
			// projection along track to ensure crossing near the start segment
			float proj = (curr.x - s0.x) * fx + (curr.z - s0.z) * fz;
			float allowable = g_track.width * 0.7f; // half-length threshold along track
			// movement across the line (sign change) and within allowable projection and moving forward
			float movAlong = (curr.x - prev.x) * fx + (curr.z - prev.z) * fz;
			if (dPrev > 0.0f && dCurr <= 0.0f && fabsf(proj) < allowable && movAlong > 0.0f && (g_totalTime - g_lastCrossTime) > 1.0f) {
				g_laps += 1;
				g_lastCrossTime = g_totalTime;
				g_lapTime = 0.0f;
				// Log crossing to console and file for debugging
				char logBuf[256];
				std::snprintf(logBuf, sizeof(logBuf), "[Lap %d] time=%.2f pos=(%.2f,%.2f)\n", g_laps, g_totalTime, curr.x, curr.z);
				std::cout << logBuf;
				std::ofstream lf("lap_log.txt", std::ios::app);
				if (lf) lf << logBuf;
			}
			// update prev pos for next frame
			g_prevCarPos = curr;
		}
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

		// draw HUD (2D overlay)
		HUD::drawHUD(g_car, g_laps, g_lapTime, g_totalTime);
	}
}
