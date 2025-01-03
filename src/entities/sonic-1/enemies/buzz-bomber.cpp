#include "EnemiesGHZ.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include <cmath>
#include <vector>

void EnBuzz::init() {
    dv_hitBoxSize = v2f(48, 24);
    moveTimer = 192;
    dv_type = TYPE_ENEMY;
    // dv_anim.create(TEX_OBJECTS);
    // animWings.create(TEX_OBJECTS);
    // animWings.set(119, 120, 0.75);
    // dv_anim.set(117, 117, 0);

    animator_.setSpeed(0.0f);///
    animatorWings_.setSpeed(0.75f);///
    animatorTurbo_.setSpeed(0.25f);///
    animatorFire_.setSpeed(0.0f);///
}

void EnBuzz::d_update() {
    if (moveTimer <= 0) {
        if (idleTimer > 0) {
            animator_.setCurrentFrame(1.0f);
            if(1);
            idleTimer--;
            xsp = 0.0;
            if (idleTimer <= 34 && fired) {
                animator_.setSpeed(0.0f);
            }
        } else {
            animator_.setCurrentFrame(0.0f);
            animator_.setSpeed(0.0f);
            if (moveTimer == 0){
                fired = false;
            }
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
    animator_.tick();
    animatorWings_.tick();
    animatorTurbo_.tick();
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
                new Bullet(v2f(dv_pos.x - 17, dv_pos.y + 23),animatorBullet_, 0, -1));
        else
            entities.push_back(
                new Bullet(v2f(dv_pos.x + 17, dv_pos.y + 23),animatorBullet_, 0, 1));
    }
}

void EnBuzz::d_draw(Camera &cam) {
    v2f tPos = v2f(dv_pos.x + 16 * (1-2*faceRight), dv_pos.y+6);
    v2f fPos = v2f(dv_pos.x - 17 * (1-2*faceRight),  dv_pos.y+22);
    v2f wPos;
    if (int(animator_.getCurrentFrameIndex()) == 118)
        wPos = v2f(dv_pos.x + 2 * (1-2*faceRight), dv_pos.y - 12);
    else
        wPos = v2f(dv_pos.x - 4 * (1-2*faceRight), dv_pos.y - 9);

    

    auto &spr = animator_.getCurrentFrame();
    auto &sprWings = animatorWings_.getCurrentFrame();
    auto &sprTurbo = animatorTurbo_.getCurrentFrame();
    auto &sprFire = animatorFire_.getCurrentFrame();
    cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                           .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=faceRight});
    cam.getScr().artist().drawSprite(sprWings, {.x = wPos.x - cam.getPos().x,
                                                .y = wPos.y - cam.getPos().y}, {.flipHorizontal=faceRight});
    if(xsp!=0)
        cam.getScr().artist().drawSprite(sprTurbo, {.x = tPos.x - cam.getPos().x,
                                                    .y = tPos.y - cam.getPos().y}, {.flipHorizontal=faceRight});
    cam.getScr().artist().drawSprite(sprFire, {.x = fPos.x - cam.getPos().x,
                                               .y = fPos.y - cam.getPos().y}, {.flipHorizontal=faceRight});
}

