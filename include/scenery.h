#ifndef SCENERY_H
#define SCENERY_H

#include "track.h"
#include <vector>

class Scenery {
public:
    Scenery();
    // generate scenery using the provided track sampled centerline
    void initWithTrack(const Track* track);
    void update(float dt) {}
    void draw() const;

private:
    // simple placement lists
    std::vector<Vec3> treePositions;
    const Track* sourceTrack;

    void generateFromTrack();
    void drawTree(const Vec3 &pos) const;
};

#endif // SCENERY_H
