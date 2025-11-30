// Track generation and rendering (Catmull-Rom smoothing)
#include "../../include/track.h"
#include <GL/freeglut.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <fstream>

static float clampf(float v, float a, float b) { return (v<a)?a: (v>b)?b: v; }

static Vec3 catmullRom(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, float t) {
    // Catmull-Rom spline (centripetal form omitted for simplicity)
    float t2 = t * t;
    float t3 = t2 * t;
    Vec3 res;
    res.x = 0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * t + (2.0f*p0.x - 5.0f*p1.x + 4.0f*p2.x - p3.x)*t2 + (-p0.x + 3.0f*p1.x - 3.0f*p2.x + p3.x)*t3);
    res.y = 0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * t + (2.0f*p0.y - 5.0f*p1.y + 4.0f*p2.y - p3.y)*t2 + (-p0.y + 3.0f*p1.y - 3.0f*p2.y + p3.y)*t3);
    res.z = 0.5f * ((2.0f * p1.z) + (-p0.z + p2.z) * t + (2.0f*p0.z - 5.0f*p1.z + 4.0f*p2.z - p3.z)*t2 + (-p0.z + 3.0f*p1.z - 3.0f*p2.z + p3.z)*t3);
    return res;
}

Track::Track() : width(12.0f) {}

void Track::setControlPoints(const std::vector<Vec3> &pts) {
    controlPoints = pts;
    buildSampled();
}

void Track::initDefault() {
    // a simple oval with some variation
    controlPoints.clear();
    const float rx = 18.0f, rz = 10.0f;
    for (int i = 0; i < 12; ++i) {
        float a = (float)i / 12.0f * 2.0f * 3.14159265f;
        Vec3 p{ rx * cosf(a), 0.0f, rz * sinf(a) };
        // add a slight bump
        p.y = 0.0f;
        controlPoints.push_back(p);
    }
    buildSampled(18);
}

bool Track::loadFromFile(const std::string &path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open track file: " << path << std::endl;
        return false;
    }

    controlPoints.clear();
    float x, y, z;

    while (f >> x >> y >> z) {
        controlPoints.push_back({x, y, z});
    }

    f.close();

    if (controlPoints.size() < 4) {
        std::cerr << "Track file requires at least 4 control points\n";
        return false;
    }

    buildSampled();   // tạo đường mượt
    return true;
}

void Track::buildSampled(int samplesPerSegment) {
    sampled.clear();
    int n = (int)controlPoints.size();
    if (n < 4) return;
    for (int i = 0; i < n; ++i) {
        const Vec3 &p0 = controlPoints[(i - 1 + n) % n];
        const Vec3 &p1 = controlPoints[i];
        const Vec3 &p2 = controlPoints[(i + 1) % n];
        const Vec3 &p3 = controlPoints[(i + 2) % n];
        for (int s = 0; s < samplesPerSegment; ++s) {
            float t = (float)s / (float)samplesPerSegment;
            Vec3 pos = catmullRom(p0,p1,p2,p3,t);
            sampled.push_back(pos);
        }
    }
}

void Track::draw() const {
    if (sampled.empty()) return;

    // Draw road surface using triangle strip
    // allow lighting; set material for road (darker, low spec)
    GLfloat roadSpec[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, roadSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_TRIANGLE_STRIP);
    for (size_t i = 0; i < sampled.size(); ++i) {
        const Vec3 &p = sampled[i];
        // compute forward tangential
        const Vec3 &pNext = sampled[(i + 1) % sampled.size()];
        Vec3 forward{pNext.x - p.x, pNext.y - p.y, pNext.z - p.z};
        // horizontalize
        forward.y = 0.0f;
        float flen = sqrtf(forward.x*forward.x + forward.z*forward.z);
        if (flen == 0) flen = 1.0f;
        forward.x /= flen; forward.z /= flen;
        // left normal = cross(up, forward)
        Vec3 left{ -forward.z, 0.0f, forward.x };
        float half = width * 0.5f;
        Vec3 leftP{ p.x + left.x * half, p.y, p.z + left.z * half };
        Vec3 rightP{ p.x - left.x * half, p.y, p.z - left.z * half };

        glVertex3f(leftP.x, leftP.y + 0.01f, leftP.z);
        glVertex3f(rightP.x, rightP.y + 0.01f, rightP.z);
    }
    // close strip
    // repeat first two verts to close
    const Vec3 &p0 = sampled[0];
    const Vec3 &p1 = sampled[1 % sampled.size()];
    Vec3 f{p1.x - p0.x, 0.0f, p1.z - p0.z};
    float fl = sqrtf(f.x*f.x + f.z*f.z); if (fl==0) fl=1.0f; f.x/=fl; f.z/=fl;
    Vec3 lf{-f.z,0.0f,f.x};
    glVertex3f(p0.x + lf.x*width*0.5f, p0.y+0.01f, p0.z + lf.z*width*0.5f);
    glVertex3f(p0.x - lf.x*width*0.5f, p0.y+0.01f, p0.z - lf.z*width*0.5f);
    glEnd();

    // Draw borders (white lines)
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (size_t i=0;i<sampled.size();++i) {
        const Vec3 &p = sampled[i];
        const Vec3 &pNext = sampled[(i + 1) % sampled.size()];
        Vec3 forward{pNext.x - p.x, 0.0f, pNext.z - p.z};
        float flen2 = sqrtf(forward.x*forward.x + forward.z*forward.z); if (flen2==0) flen2=1.0f; forward.x/=flen2; forward.z/=flen2;
        Vec3 left{ -forward.z, 0.0f, forward.x };
        Vec3 leftP{ p.x + left.x * width*0.5f, p.y, p.z + left.z * width*0.5f };
        glVertex3f(leftP.x, leftP.y + 0.02f, leftP.z);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (size_t i=0;i<sampled.size();++i) {
        const Vec3 &p = sampled[i];
        const Vec3 &pNext = sampled[(i + 1) % sampled.size()];
        Vec3 forward{pNext.x - p.x, 0.0f, pNext.z - p.z};
        float flen2 = sqrtf(forward.x*forward.x + forward.z*forward.z); if (flen2==0) flen2=1.0f; forward.x/=flen2; forward.z/=flen2;
        Vec3 left{ -forward.z, 0.0f, forward.x };
        Vec3 rightP{ p.x - left.x * width*0.5f, p.y, p.z - left.z * width*0.5f };
        glVertex3f(rightP.x, rightP.y + 0.02f, rightP.z);
    }
    glEnd();

    // Start/Finish quad at sampled[0]
    const Vec3 &s = sampled[0];
    const Vec3 &sNext = sampled[1 % sampled.size()];
    Vec3 forward{ sNext.x - s.x, 0.0f, sNext.z - s.z };
    float flen3 = sqrtf(forward.x*forward.x + forward.z*forward.z); if (flen3==0) flen3=1.0f; forward.x/=flen3; forward.z/=flen3;
    Vec3 left{ -forward.z, 0.0f, forward.x };
    float sfw = width * 0.9f;
    Vec3 a{ s.x + left.x*sfw*0.5f, s.y+0.03f, s.z + left.z*sfw*0.5f };
    Vec3 b{ s.x - left.x*sfw*0.5f, s.y+0.03f, s.z - left.z*sfw*0.5f };
    Vec3 c{ s.x - left.x*sfw*0.5f + forward.x*1.0f, s.y+0.03f, s.z - left.z*sfw*0.5f + forward.z*1.0f };
    Vec3 d{ s.x + left.x*sfw*0.5f + forward.x*1.0f, s.y+0.03f, s.z + left.z*sfw*0.5f + forward.z*1.0f };
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(d.x, d.y, d.z);
    glEnd();
}
