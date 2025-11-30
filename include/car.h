#ifndef CAR_H
#define CAR_H

#include "camera.h" // for Vec3

class Car {
public:
    Car();

    // update physics & controls
    void update(float dt);

    // render the car at its position
    void draw() const;

    void reset();

    Vec3 position;
    float yaw;   // degrees
    float speed; // units per second

private:
    float m_acceleration; // units/s^2
    float m_maxSpeed;
    float m_turnSpeed; // degrees per second at full speed
    float m_friction; // deceleration per second
};

#endif // CAR_H
