#include "EnemiesGHZ.h"

// Chopper
void EnChopper::init() {
    dv_hitBoxSize = v2f(26, 34);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(111, 112, 0.2f);
}

void EnChopper::d_update() {
    dv_pos.y += ysp;

    if (dv_pos.y >= dv_startPos.y - 24)
        ysp = -7;
    ysp += 0.09375;

    dv_anim.tick();
}