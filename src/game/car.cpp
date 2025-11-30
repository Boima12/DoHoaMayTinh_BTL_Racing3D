// Simple Car implementation (cube body + 4 wheel blocks)
#include "../../include/car.h"
#include "../../include/input.h"
#include <GL/freeglut.h>
#include <cmath>

static float deg2rad(float d) { return d * 3.14159265f / 180.0f; }

Car::Car()
    : position{0.0f, 0.0f, 0.0f}, yaw(0.0f), speed(0.0f),
    // m_acceleration(6.0f), m_maxSpeed(12.0f), m_turnSpeed(90.0f), m_friction(4.0f)
        m_acceleration(9.0f), m_maxSpeed(18.0f), m_turnSpeed(90.0f), m_friction(6.0f)
{
}

void Car::reset() {
    position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;
    yaw = 0.0f; speed = 0.0f;
}

void Car::update(float dt) {
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

    if (Input::isKeyDown(' ')) {
        reset();
    }

    // Integrate position
    float yRad = deg2rad(yaw);
    position.x += sinf(yRad) * speed * dt;
    position.z += cosf(yRad) * speed * dt;
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
