// Scenery: simple trees and lamp posts placed around the track
#include "../../include/scenery.h"
#include "../../include/config.h"
#include <GL/freeglut.h>
#include <cmath>

Scenery::Scenery() : sourceTrack(nullptr) {}

void Scenery::initWithTrack(const Track* track) {
    sourceTrack = track;
    treePositions.clear();
    if (sourceTrack) generateFromTrack();
}

void Scenery::generateFromTrack() {
    if (!sourceTrack) return;
    const auto &s = sourceTrack->sampled;
    if (s.empty()) return;

    // place a tree every N samples on alternating sides, and a lamp every M samples
    int N_tree = Config::N_tree;
    float offset = sourceTrack->width * 0.7f;

    for (size_t i = 0; i < s.size(); ++i) {
        const Vec3 &p = s[i];
        const Vec3 &pnext = s[(i + 1) % s.size()];
        // compute forward and left
        Vec3 forward{pnext.x - p.x, 0.0f, pnext.z - p.z};
        float flen = sqrtf(forward.x*forward.x + forward.z*forward.z);
        if (flen == 0.0f) continue;
        forward.x /= flen; forward.z /= flen;
        Vec3 left{-forward.z, 0.0f, forward.x};

        if ((i % N_tree) == 0) {
            float side = ((i / N_tree) % 2 == 0) ? 1.0f : -1.0f;
            Vec3 t{ p.x + left.x * (offset + 1.0f) * side,
                    p.y,
                    p.z + left.z * (offset + 1.0f) * side };
            treePositions.push_back(t);
        }
    }
}

void Scenery::drawTree(const Vec3 &pos) const {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    // trunk
    // trunk (darker, low spec)
    GLfloat trunkSpec[] = {0.02f, 0.02f, 0.02f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, trunkSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 4.0f);
    glPushMatrix();
        glTranslatef(0.0f, 0.8f, 0.0f);
        glColor3f(0.45f, 0.25f, 0.07f);
        glScalef(0.2f, 1.6f, 0.2f);
        glutSolidCube(1.0f);
    glPopMatrix();
    // foliage (3 stacked spheres)
    // foliage (green, slightly glossy)
    GLfloat leafSpec[] = {0.05f, 0.15f, 0.05f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, leafSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 12.0f);
    glColor3f(0.0f, 0.45f, 0.0f);
    glPushMatrix();
        glTranslatef(0.0f, 1.8f, 0.0f);
        glutSolidSphere(0.6f, 12, 8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, 2.2f, 0.0f);
        glutSolidSphere(0.45f, 12, 8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, 2.6f, 0.0f);
        glutSolidSphere(0.35f, 12, 8);
    glPopMatrix();
    glPopMatrix();
}

void Scenery::draw() const {
    // draw trees
    for (const auto &t : treePositions) drawTree(t);
}
