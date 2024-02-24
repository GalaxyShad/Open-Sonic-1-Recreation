#include "EnemiesGHZ.h"

void EnBuzz::init() {
    dv_hitBoxSize = v2f(48, 24);
    moveTimer = 192;
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    animWings.create(TEX_OBJECTS);
    animWings.set(119, 120, 0.75);
    dv_anim.set(117, 117, 0);
}

void EnBuzz::d_update() {
    if (moveTimer <= 0) {
        if (idleTimer > 0) {
            idleTimer--;
            xsp = 0.0;
            if (idleTimer <= 34 && fired) {
                dv_anim.set(118, 118, 0);
            }
        } else {
            dv_anim.set(117, 117, 0);
            if (moveTimer == 0)
                fired = false;
            if (!fired) {
                if (faceRight)
                    faceRight = false;
                else
                    faceRight = true;
            }
            moveTimer = 128;
        }
    } else {
        if (faceRight)
            xsp = BUZZ_SPD;
        else
            xsp = -BUZZ_SPD;
        moveTimer--;
        idleTimer = 0;
    }

    dv_pos.x += xsp;
    dv_anim.tick();
    animWings.tick();
}

void EnBuzz::d_reactingToOthers(std::list<Entity *> &entities) {
    // Player Detection
    std::list<Entity *>::iterator it;
    for (it = entities.begin(); it != entities.end(); it++) {
        if ((*it)->d_getType() == TYPE_PLAYER) {
            if (d_entMeeting(*(*it), v2i(48, 96)) && !fired) {
                moveTimer = -1;
                idleTimer = 48;

                fired = true;
            }
            break;
        }
    }
    if (fired && idleTimer == 24) {
        if (!faceRight)
            entities.push_back(
                new Bullet(v2f(dv_pos.x - 17, dv_pos.y + 23), 0, -1));
        else
            entities.push_back(
                new Bullet(v2f(dv_pos.x + 17, dv_pos.y + 23), 0, 1));
    }
}

void EnBuzz::d_draw(Camera &cam) {
    v2f wPos;

    if (faceRight) {
        if (int(dv_anim.getCurFrame()) == 118)
            wPos = v2f(dv_pos.x - 2, dv_pos.y - 12);
        else
            wPos = v2f(dv_pos.x + 4, dv_pos.y - 9);
    } else {
        if (int(dv_anim.getCurFrame()) == 118)
            wPos = v2f(dv_pos.x + 2, dv_pos.y - 12);
        else
            wPos = v2f(dv_pos.x - 4, dv_pos.y - 9);
    }

    cam.draw(animWings, wPos, 0.0, faceRight);
    cam.draw(dv_anim, dv_pos, 0.0, faceRight);
}

