#ifndef HUD_H
#define HUD_H

#include "car.h"

namespace HUD {
    // draw HUD overlay: speed, laps, lap/total time and hints
    void drawHUD(const Car &car, int laps, float lapTime, float totalTime);
}

#endif // HUD_H
