#include "../../include/camera.h"
#include "../../include/input.h"
#include <GL/freeglut.h>
#include <cmath>

static float deg2rad(float d) { return d * 3.14159265f / 180.0f; }
static float clampf(float v, float a, float b) { return (v<a)?a: (v>b)?b: v; }

Camera::Camera()
	: m_pos{0.0f, 2.0f, 6.0f}, m_target{0.0f, 0.0f, 0.0f}, m_yaw(0.0f), m_pitch(-15.0f),
	  m_distance(6.0f), m_height(1.5f), m_damping(6.0f)
{
}

void Camera::setDistance(float d) {
	m_distance = clampf(d, 1.0f, 50.0f);
}

void Camera::rotateYaw(float deg) { m_yaw += deg; }
void Camera::rotatePitch(float deg) { m_pitch = clampf(m_pitch + deg, -89.0f, 89.0f); }

void Camera::update(const Vec3 &target, float dt) {
	// apply input controls: arrow keys rotate camera
	if (Input::isSpecialDown(GLUT_KEY_LEFT)) rotateYaw(-80.0f * dt);
	if (Input::isSpecialDown(GLUT_KEY_RIGHT)) rotateYaw(80.0f * dt);
	if (Input::isSpecialDown(GLUT_KEY_UP)) rotatePitch(40.0f * dt);
	if (Input::isSpecialDown(GLUT_KEY_DOWN)) rotatePitch(-40.0f * dt);

	// mouse wheel zoom
	int wheel = Input::wheelDelta();
	if (wheel != 0) {
		setDistance(m_distance - wheel * 0.5f);
	}

	// compute desired camera position in spherical coordinates around target
	float yawRad = deg2rad(m_yaw);
	float pitchRad = deg2rad(m_pitch);

	float cosPitch = cosf(pitchRad);
	Vec3 desired;
	desired.x = target.x + m_distance * sinf(yawRad) * cosPitch * -1.0f;
	desired.z = target.z + m_distance * cosf(yawRad) * cosPitch;
	desired.y = target.y + m_distance * sinf(pitchRad) + m_height;

	// smoothing (simple lerp)
	float alpha = clampf(dt * m_damping, 0.0f, 1.0f);
	m_pos.x = m_pos.x * (1 - alpha) + desired.x * alpha;
	m_pos.y = m_pos.y * (1 - alpha) + desired.y * alpha;
	m_pos.z = m_pos.z * (1 - alpha) + desired.z * alpha;

	m_target = target;
}

void Camera::applyView() const {
	gluLookAt(m_pos.x, m_pos.y, m_pos.z,
			  m_target.x, m_target.y, m_target.z,
			  0.0f, 1.0f, 0.0f);
}
