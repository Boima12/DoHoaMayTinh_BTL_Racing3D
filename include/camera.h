#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"

struct Vec3 {
	float x, y, z;
};

class Camera {
public:
	Camera();

	void setDistance(float d);
	float distance() const { return m_distance; }

	// update camera to follow target position (world coords)
	void update(const Vec3 &target, float dt);

	// apply view transform (gluLookAt)
	void applyView() const;

	// manual control
	void rotateYaw(float deg);
	void rotatePitch(float deg);

	// snap camera to a specific world position and target immediately
	void snapTo(const Vec3 &pos, const Vec3 &target);

private:
	Vec3 m_pos;       // camera world position
	Vec3 m_target;    // target to look at
	float m_yaw;      // degrees around Y
	float m_pitch;    // degrees up/down
	float m_distance; // desired follow distance
	float m_height;   // camera height above target
	float m_damping;  // smoothing factor
};

#endif // CAMERA_H

