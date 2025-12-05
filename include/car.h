#ifndef CAR_H
#define CAR_H

#include "camera.h" // for Vec3

class Car {
public:
    Car();

    // update physics & controls; pass Track to enforce bounds
    void update(float dt, const class Track* track);

    // render the car at its position
    void draw() const;

    void reset();
    // reset car to start position defined by track (optional back offset)
    void resetToTrack(const class Track* track, float backOffset = 1.5f);

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
