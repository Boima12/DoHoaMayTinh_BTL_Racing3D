#include "../../include/car.h"
#include "../../include/track.h"
#include "../../include/input.h"
#include <GL/freeglut.h>
#include <cmath>

static float deg2rad(float d) { return d * 3.14159265f / 180.0f; }

Car::Car()
    : position{0.0f, 0.0f, 0.0f}, yaw(0.0f), speed(0.0f),
        m_acceleration(11.0f), m_maxSpeed(22.0f), m_turnSpeed(90.0f), m_friction(6.0f)
        // m_acceleration(18.0f), m_maxSpeed(50.0f), m_turnSpeed(180.0f), m_friction(6.0f)
{
}

void Car::reset() {
    position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;
    yaw = 0.0f; speed = 0.0f;
}

void Car::resetToTrack(const Track* track, float backOffset) {
    // default to zero if no track
    if (!track || track->sampled.empty()) {
        reset();
        return;
    }
    const Vec3 &s0 = track->sampled[0];
    const Vec3 &s1 = track->sampled[(1) % track->sampled.size()];
    float fx = s1.x - s0.x;
    float fz = s1.z - s0.z;
    float flen = sqrtf(fx*fx + fz*fz); if (flen == 0.0f) flen = 1.0f;
    fx /= flen; fz /= flen;
    float yawRad = atan2f(fx, fz);
    yaw = yawRad * 180.0f / 3.14159265f;
    position.x = s0.x - fx * backOffset;
    position.y = s0.y;
    position.z = s0.z - fz * backOffset;
    speed = 0.0f;
}

void Car::update(float dt, const Track* track) {
    // Controls: W/S forward/back, A/D turn, SPACE reset
    if (Input::isKeyDown('w') || Input::isKeyDown('W')) {
        speed += m_acceleration * dt;
    } else if (Input::isKeyDown('s') || Input::isKeyDown('S')) {
        speed -= m_acceleration * dt;
    } else {
        // natural friction
        if (speed > 0.0f) {
            speed -= m_friction * dt;
            if (speed < 0.0f) speed = 0.0f;
        } else if (speed < 0.0f) {
            speed += m_friction * dt;
            if (speed > 0.0f) speed = 0.0f;
        }
    }

    // clamp speed
    if (speed > m_maxSpeed) speed = m_maxSpeed;
    if (speed < -m_maxSpeed*0.5f) speed = -m_maxSpeed*0.5f; // reverse slower

    // Turning: scale by speed (when stationary, less turning)
    float speedFactor = (fabsf(speed) / m_maxSpeed);
    if (speedFactor < 0.1f) speedFactor = 0.1f;
    if (Input::isKeyDown('a') || Input::isKeyDown('A')) {
        yaw += m_turnSpeed * dt * (speed >= 0 ? 1.0f : -1.0f) * speedFactor;
    }
    if (Input::isKeyDown('d') || Input::isKeyDown('D')) {
        yaw -= m_turnSpeed * dt * (speed >= 0 ? 1.0f : -1.0f) * speedFactor;
    }

    // if (Input::isKeyDown(' ')) {
        // removed direct reset here; reset now handled by Game to also reset timers
    // }

    // Integrate position
    float yRad = deg2rad(yaw);
    position.x += sinf(yRad) * speed * dt;
    position.z += cosf(yRad) * speed * dt;

    // enforce track bounds if track provided
    if (track) {
        Vec3 closest;
        float dist = track->distanceToCenter(position, closest);
        float half = track->width * 0.5f;
        if (dist > half) {
            // outside track: clamp position to boundary and reduce speed
            float dx = position.x - closest.x;
            float dz = position.z - closest.z;
            float len = sqrtf(dx*dx + dz*dz);
            if (len > 1e-6f) {
                dx /= len; dz /= len;
                // place slightly inside border
                position.x = closest.x + dx * (half - 0.01f);
                position.z = closest.z + dz * (half - 0.01f);
            } else {
                // exactly at center? push to edge along yaw
                position.x = closest.x + sinf(yRad) * (half - 0.01f);
                position.z = closest.z + cosf(yRad) * (half - 0.01f);
            }
            // reduce speed
            speed *= 0.5f;
        }
    }
}

void Car::draw() const {
    glPushMatrix();
    glTranslatef(position.x, position.y + 0.4f, position.z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);

    // body
    // set specular for car body
    GLfloat carSpec[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, carSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
    glPushMatrix();
        glScalef(1.6f, 0.6f, 3.0f);
        glColor3f(0.8f, 0.1f, 0.1f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // small yellow front marker on hood to indicate forward direction
    glPushMatrix();
        // place slightly above the car body and near the front (+z)
        // body half-height = 0.6f * 0.5 = 0.3f, so place marker at y ~= 0.35
        // front z position roughly half-length minus a little padding: 3.0f*0.5 - 0.15 = 1.35f
        glTranslatef(0.0f, 0.35f, 1.35f);
        GLfloat markerSpec[] = {0.9f, 0.9f, 0.2f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, markerSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
        glColor3f(1.0f, 0.9f, 0.0f);
        // small rounded bump
        glutSolidSphere(0.14f, 12, 8);
    glPopMatrix();

    // wheels (as small cubes)
    // wheels: darker, lower specular
    GLfloat wheelSpec[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, wheelSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 4.0f);
    glColor3f(0.05f, 0.05f, 0.05f);
    const float wx = 0.9f, wz = 1.1f, wy = -0.25f;
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
                glTranslatef(i * wx, wy, j * wz);
                glScalef(0.4f, 0.4f, 0.8f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    glPopMatrix();
}
