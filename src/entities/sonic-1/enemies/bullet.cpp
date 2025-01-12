#include "EnemiesGHZ.h"
#include "core/GameMath.h"

void Bullet::init() {
    dv_hitBoxSize = v2f(14, 14);
    dv_type = TYPE_BULLET;
    if (mode == 0) {
        xsp = 2 * dir;
        ysp = 2;
    } else {
        ysp = -4;
        xsp = 1 * dir;
    }
    animator_.setSpeed(0.2f);
}

void Bullet::d_update() {
    if (dv_anim.getCurFrame() >= 127)
        animator_.setSpeed(0.2f);

    dv_pos.x += xsp;
    dv_pos.y += ysp;

    if (mode != 0)
        ysp += grv;

    animator_.tick();
}

void Bullet::d_draw(Camera &cam) {
    bool flip = false;
    if (dir == 1)
        flip = true;

    auto &spr = animator_.getCurrentFrame();
    cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                           .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=flip});
}