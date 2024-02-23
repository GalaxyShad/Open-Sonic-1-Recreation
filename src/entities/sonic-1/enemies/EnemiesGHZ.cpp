#include "EnemiesGHZ.h"
#include "core/GameMath.h"

// Bullet
void Bullet::init() {
    dv_hitBoxSize = v2f(14, 14);
    dv_type = TYPE_BULLET;
    dv_anim.create(TEX_OBJECTS);
    if (mode == 0) {
        dv_anim.set(123, 127, 0.2f);
        xsp = 2 * dir;
        ysp = 2;
    } else {
        dv_anim.set(126, 127, 0.2f);
        ysp = -4;
        xsp = 1 * dir;
    }
}

void Bullet::d_update() {
    if (dv_anim.getCurFrame() >= 127)
        dv_anim.set(126, 127, 0.2f);

    dv_pos.x += xsp;
    dv_pos.y += ysp;

    if (mode != 0)
        ysp += grv;

    dv_anim.tick();
}

void Bullet::d_draw(Camera &cam) {
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(dv_anim, dv_pos, 0.0, flip);
}

// Motobug
void EnMotobug::init() {
    dv_hitBoxSize = v2f(42, 34);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(134, 137, 0.2f);
}

void EnMotobug::d_update() {
    dv_pos.x += MOTOBUG_SPD * dir;
    dv_anim.tick();
    trnCollision();
}

void EnMotobug::d_draw(Camera &cam) {
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(dv_anim, dv_pos, 0.0, flip);

    m_sensor.draw(cam);
}

void EnMotobug::trnCollision() {
    m_sensor.setPosition(v2f(dv_pos.x, dv_pos.y + 14));

    auto distToGround = m_sensor.getDistance();
    bool colliding = (distToGround >= -8) && (distToGround <= 12);

    if (colliding) {
        dv_pos.y += distToGround;
    } else {
        dir = -dir;
    }
}

// Crab
void EnCrab::init() {
    dv_hitBoxSize = v2f(42, 30);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(113, 115, 0.2, true);
}

void EnCrab::d_update() {
    if (moveTimer == 0) {
        dv_anim.set(113, 115, 0.1, true);
        if (faceRight)
            xsp = CRAB_SPD;
        else
            xsp = -CRAB_SPD;
    } else {
        moveTimer--;
        xsp = 0.0;
        if (moveTimer >= 25) {
            dv_anim.set(113, 113, 0);
            bulFlag = false;
        } else {
            dv_anim.set(116, 116, 0);
            if (!bulFlag) {
                entities->push_back(
                    new Bullet(v2f(dv_pos.x - 18, dv_pos.y - 8), 1, -1));
                entities->push_back(
                    new Bullet(v2f(dv_pos.x + 18, dv_pos.y - 8), 1, 1));
                bulFlag = true;
            }
        }
    }

    if (dv_pos.x > dv_startPos.x + 72) {
        moveTimer = 50;
        faceRight = false;
        xsp *= -1;
    }
    if (dv_pos.x < dv_startPos.x - 72) {
        moveTimer = 50;
        faceRight = true;
        xsp *= -1;
    }

    dv_pos.x += xsp;
    dv_anim.tick();

    m_tick++;

    trnCollision();
}

void EnCrab::trnCollision() {
    m_sensor.setPosition(v2f(
        dv_pos.x + 8 * (m_tick % 2 == 0) * gmath::fsign(xsp), dv_pos.y + 16));

    auto distToGround = m_sensor.getDistance();
    bool colliding = (distToGround >= -8) && (distToGround <= 12);

    if (colliding) {
        dv_pos.y += distToGround;
    } else {
        moveTimer = 50;
        faceRight = !faceRight;
        xsp = -xsp;
    }
}

// BUZZ
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