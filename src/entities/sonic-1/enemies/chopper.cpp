#include "EnemiesGHZ.h"

// Chopper
void EnChopper::init() {
    dv_hitBoxSize = v2f(26, 34);
    dv_type = TYPE_ENEMY;

    animator_.setSpeed(0.2f);
}

void EnChopper::d_update() {
    dv_pos.y += ysp;

    if (dv_pos.y >= dv_startPos.y - 24)
        ysp = -7;
    ysp += 0.09375;

    animator_.tick();
}