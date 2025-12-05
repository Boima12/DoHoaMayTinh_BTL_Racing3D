#ifndef TRACK_H
#define TRACK_H

#include "camera.h"
#include <vector>
#include <string>

class Track {
public:
	Track();
	// initialize a default sample track (Catmull-Rom smoothed loop)
	void initDefault();

	// optional: set control points directly
	void setControlPoints(const std::vector<Vec3> &pts);

    // load track from file
    bool loadFromFile(const std::string &path);

	void update(float dt) {}
	void draw() const;

	// access to raw sampled centerline (useful for edits)
	std::vector<Vec3> sampled;
	float width;
	// compute nearest point on sampled centerline (2D xz), returns distance and sets outClosest
	float distanceToCenter(const Vec3 &p, Vec3 &outClosest) const;

private:
	std::vector<Vec3> controlPoints;
	void buildSampled(int samplesPerSegment = 12);
};

#endif // TRACK_H
