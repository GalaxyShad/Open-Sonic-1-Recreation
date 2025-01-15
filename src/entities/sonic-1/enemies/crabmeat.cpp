#include "EnemiesGHZ.h"

void EnCrab::init() {
    dv_hitBoxSize = v2f(42, 30);
    dv_type = TYPE_ENEMY;
    animator_.setSpeed(0.2f);
}

void EnCrab::d_update() {
    if (moveTimer == 0) {
        animator_.setSpeed(0.1f);
        if (faceRight)
            xsp = CRAB_SPD;
        else
            xsp = -CRAB_SPD;
    } else {
        moveTimer--;
        xsp = 0.0;
        if (moveTimer >= 25) {
            animator_.setSpeed(0.0f);
            bulFlag = false;
        } else {
            animator_.setSpeed(0.0f);
            if (!bulFlag) {
                entities->push_back(
                    new Bullet(v2f(dv_pos.x - 18, dv_pos.y - 8), animationBullet_, 1, -1));
                entities->push_back(
                    new Bullet(v2f(dv_pos.x + 18, dv_pos.y - 8), animationBullet_, 1, 1));
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
    animator_.tick();

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