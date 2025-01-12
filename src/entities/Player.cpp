#include "Player.h"
#include "core/game_enviroment/artist/SpriteFont.h"
#include "entities/Entity.h"
#include "entities/Player.h"
#include "player-sensor.hpp"

#include "player-state-jump.hpp"
#include "player-state-machine.hpp"
#include "player-state-normal.hpp"
#include "sonic/SonicResources.h"

void Player::onHitboxCollision(Entity &entity) {
    if (entity.type() == EntityTypeID::RING) {
        rings++;
        audio.dj().playSound(sndRing_);
        m_entityPool.destroy(entity);
    }
}

// === public === //
void Player::init() {
    dv_type = TYPE_PLAYER;
    dv_hitBoxSize = v2f(20, 40);
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(0, 0, 0);
    standOn = nullptr;

    m_stateMachine.add(new PlayerStateNormal(m_props, sndSkid_));
    m_stateMachine.add(new PlayerStateJump(m_props, 6.5, sndJump_));
    m_stateMachine.add(new PlayerStateRoll(m_props, sndRoll_));
    m_stateMachine.add(new PlayerStateSkid(m_props, sndSkid_));
    m_stateMachine.add(new PlayerStateSpindashCD(m_props, sndRoll_));
    m_stateMachine.add(new PlayerStateSpring(m_props));
    m_stateMachine.add(new PlayerStateHurt(m_props, sndRingLoss_));
    m_stateMachine.add(new PlayerStateDie(m_props, sndRingLoss_));

    m_collider.onLanding(
        [this](HexAngle hexAngle) { m_stateMachine.pushLanding(); });
}

void Player::d_update() {
    m_stateMachine.update();

    m_collider.setRadius(m_stateMachine.isCurling() ? v2i(7, 14) : v2i(9, 19));

    movement();
    if (!debug)
        m_collider.update();
    angle = m_collider.getAngle().degrees();

    gameplay();
    animation();
    dv_anim.tick();

    terrainCollision(cam);
    moveCam(cam);
    entitiesCollision(entities, cam);
}

void Player::d_draw(Camera &cam) {

    // Animation angle
    if (animAngle == 360)
        animAngle = 0;

    if (invicTimer == 0 || (invicTimer > 0 && invicTimer % 8 >= 4) ||
        m_stateMachine.currentId() == PlayerStateID::HURT)
        cam.draw(dv_anim, dv_pos, anim8Angle, animFlip, false);

    char dbInfo[128];
    snprintf(dbInfo, 127,
             "position: %4.2f %4.2f\n"
             "spd:      %2.2f %2.2f\n"
             "angle:    %X(%4f)\n"
             "flr mode: %d\n"
             "action:   %d\n"
             "ground    %d\n"
             "debug     %d\n",
             dv_pos.x, dv_pos.y, spd.x, spd.y, m_collider.getAngle().hex,
             m_collider.getAngle().degrees(), m_collider.getMode(),
             m_stateMachine.currentId(), m_collider.isGrounded(), debug);

    Screen &scr = cam.getScr();

    auto fontResourceId = scr.store().map<SonicResources>().fonts.general;
    auto &font = scr.store().get<artist_api::SpriteFont>(fontResourceId);

    scr.artist().drawText(dbInfo, {.x = 427 - 256.f, .y = 8},
                          font);

    m_collider.draw(cam);
}

void Player::moveCam(Camera &cam) {
    if (m_stateMachine.currentId() == PlayerStateID::DIE)
        return;

    if (camLagTimer > 0) {
        camLagTimer--;
        return;
    }

    Size scrSize = cam.getSize();

    float _x = dv_pos.x - (float)scrSize.width / 2;
    float _y = (dv_pos.y - shiftY) - (float)scrSize.height / 2;

    v2f camPos = cam.getPos();

    float _leftBorder = camPos.x - 16;
    float _rightBorder = camPos.x;
    float _topBorder = camPos.y - 32;
    float _bottomBorder = camPos.y + 32;

    if (!endLv) {
        if (_x < _leftBorder)
            camPos.x -= fmin(_leftBorder - _x, 16.0);
        else if (_x > _rightBorder)
            camPos.x += fmin(_x - _rightBorder, 16.0);
    }

    if (m_collider.isGrounded()) {
        if (fabs(spd.y) > 6.0)
            camPos.y += fmin(fmax(_y - camPos.y, -16.0), 16.0);
        else
            camPos.y += fmin(fmax(_y - camPos.y, -6.0), 6.0);
    } else {
        if (_y < _topBorder)
            camPos.y -= fmin(_topBorder - _y, 16.0);
        else if (_y > _bottomBorder)
            camPos.y += fmin(_y - _bottomBorder, 16.0);
    }

    // cam.setPos(camPos);
    cam.setPos(v2f(camPos.x, camPos.y));
}

void Player::terrainCollision(Camera &cam) {
    if (debug)
        return;

    if (dv_pos.y + 20 > cam.getBottomBorder() && !cam.isFree()) {
        m_stateMachine.changeTo(PlayerStateID::DIE);
    }

    if (m_stateMachine.currentId() == PlayerStateID::DIE) {
        if (dv_pos.y + 20 > cam.getBottomBorder() + 40)
            dead = true;

        return;
    }

    if (endLv) {
        if (dv_pos.x < cam.getPos().x && spd.x < 0) {
            dv_pos.x = cam.getPos().x;
            gsp = 0;
            spd.x = 0;
        } else if (dv_pos.x > cam.getPos().x + cam.getSize().width &&
                   spd.x > 0) {
            dv_pos.x = cam.getPos().x + cam.getSize().width;
            gsp = 0;
            spd.x = 0;
        }
    }

    // ===== Gravity ===== //
    if (!m_collider.isGrounded()) {
        angle = 0.0;

        // Air drug
        if (!m_collider.isGrounded() && spd.y < 0.0 && spd.y > -4.0)
            spd.x -= (int(spd.x) / 0.125) / 256;

        if (spd.y < 16.0) {
            // TODO
            if (m_stateMachine.currentId() != PlayerStateID::HURT)
                spd.y += PL_GRAV;
            else
                spd.y += 0.21875;
        }
    }

    // === Fall if doesnt have enough speed ===
    // if (m_collider.isGrounded() && (fabs(gsp) < 2.5) && (flrMode !=
    // FlrMode::FLOOR)) {
    //     if (angle >= 90.0 && angle <= 270.0) {
    //         flrMode = FlrMode::FLOOR;
    //         m_collider.isGrounded() = false;
    //         angle = 0;
    //         isFalling = true;
    //         gsp = 0.0;
    //     }
    //     horizontalLockTimer = 30;
    // }

    // TODO Wha?
    if (!m_stateMachine.isCurling())
        enemyCombo = 0;
}

// TODO Make better system
void Player::entitiesCollision(std::list<Entity *> &entities, Camera &cam) {
    if (debug)
        return;

    if (m_stateMachine.currentId() == PlayerStateID::DIE)
        return;

    for (auto &it : entities) {
        auto &ent = *it;

        if (!ent.d_isInCamera(cam))
            continue;
        // Collsion Sign Post

        if (ent.d_getType() == TYPE_SIGN_POST) {
            v2f entPos = ent.d_getPos();

            if (dv_pos.x >= ent.d_getPos().x && !endLv) {
                cam.setRightBorder(ent.d_getPos().x +
                                   (float)cam.getSize().width / 2);
                cam.setBottomBorder(ent.d_getPos().y +
                                    (float)cam.getSize().height / 2);

                endLv = true;
            } else if (endLv) {
                SignPost *sp = (SignPost *)it;
                sp->setAnim(true);
            }
        }
        // Collsion Spring
        if (ent.d_getType() == TYPE_SPRING) {
            Spring *spring = (Spring *)it;
            switch (spring->getRotation()) {
            case Spring::R_UP:
                if (!d_collisionBottom(*it, 2))
                    break;

                m_stateMachine.changeTo(PlayerStateID::SPRING);

                spd.y = -(spring->isRed() ? 16 : 10);
                spring->doAnim();
                audio.dj().playSound(sndSpring_);
                break;
            case Spring::R_DOWN:
                if (!d_collisionTop(*it, 2))
                    break;

                m_stateMachine.changeTo(PlayerStateID::SPRING);

                spd.y = +(spring->isRed() ? 16 : 10);
                spring->doAnim();
                audio.dj().playSound(sndSpring_);
                break;
            case Spring::R_LEFT:
                if (!d_collisionRight(*it) || spd.x <= 0)
                    break;
                spd.x = -(spring->isRed() ? 16 : 10);
                gsp = -(spring->isRed() ? 16 : 10);
                spring->doAnim();
                audio.dj().playSound(sndSpring_);
                break;
            case Spring::R_RIGHT:
                if (!d_collisionLeft(*it) || spd.x >= 0)
                    break;
                spd.x = (spring->isRed() ? 16 : 10);
                gsp = (spring->isRed() ? 16 : 10);
                spring->doAnim();
                audio.dj().playSound(sndSpring_);
                break;
            }
        }

        if (ent.d_getType() == TYPE_SPIKES) {
            if (d_collisionBottom(*it, 2)) {
                getHit(entities);
            }
        }

        // Collision Enemy
        if (ent.d_getType() == TYPE_ENEMY) {
            Enemy *en = (Enemy *)it;
            if (d_collisionMain(*en)) {
                if (m_stateMachine.isCurling()) {
                    if (!m_collider.isGrounded()) {
                        if (spd.y > 0) {
                            spd.y *= -1;
                        } else {
                            spd.y -= sign(spd.y);
                        }
                    }
                    en->d_destroy();

                    // AnimMgr rSfx;
                    // rSfx.create(TEX_OBJECTS);
                    // rSfx.set(95, 99, 0.125);
                    // entities.push_back(new
                    // SingleAnimationEffect(v2f(en->d_getPos().x,
                    // en->d_getPos().y), rSfx));

                    enemyCombo++;
                    switch (enemyCombo) {
                    case 1:
                        score += 100;
                        break;
                    case 2:
                        score += 200;
                        break;
                    case 3:
                        score += 500;
                        break;
                    default:
                        score += 1000;
                        break;
                    }
                    entities.push_back(new EnemyScore(
                        en->d_getPos(), (EnemyScore::Points)(enemyCombo - 1)));

                    audio.dj().playSound(sndDestroy_);
                } else {
                    getHit(entities);
                }
            }
        }
        // Monitor
        if (ent.d_getType() == TYPE_MONITOR) {
            Monitor *m = (Monitor *)it;
            if (d_collisionMain(*m) && spd.y >= 0 &&
                m_stateMachine.isCurling()) {
                if (!m_collider.isGrounded()) {
                    if (spd.y > 0) {
                        spd.y *= -1;
                    } else {
                        spd.y -= sign(spd.y);
                    }
                }

                switch (m->getItem()) {
                    case Monitor::M_RINGS:
                        audio.dj().playSound(sndRing_);
                        rings += 10;
                        break;
                    case Monitor::M_SHIELD:
                        audio.dj().playSound(sndShield_);
                        break;
                    case Monitor::M_INVINCIBILITY:
                        break;
                    case Monitor::M_SPEED:
                        break;
                    case Monitor::M_LIVE:
                        break;
                }

                entities.push_back(new BrokenMonitor(m->d_getPos(),m->getAnimationBroken()));

                AnimMgr rSfx;
                rSfx.create(TEX_OBJECTS);
                rSfx.set(95, 99, 0.125);
                // entities.push_back(new SingleAnimationEffect(m->d_getPos(),
                // rSfx));

                entities.push_back(
                    new MonitorIcon(m->d_getPos(),m->getAnimationIcon(), m->getItem()));

                m->d_destroy();

                audio.dj().playSound(sndDestroy_);
            }
        }

        // Bullet
        if (ent.d_getType() == TYPE_BULLET) {
            if (d_collisionMain(*it))
                getHit(entities);
        }
        // Collision Layer Switch
        if (ent.d_getType() == TYPE_LAYER_SWITCH) {
            LayerSwitcher *ls = (LayerSwitcher *)it;
            if (d_collisionMain(*ls) && m_collider.isGrounded()) {
                if (ls->getMode() == 0 && spd.x > 0) {
                    m_collider.setLayer(terrain::TerrainLayer::NORMAL);
                } else if (ls->getMode() == 1 && spd.x < 0) {
                    m_collider.setLayer(terrain::TerrainLayer::ALTERNATE);
                } else if (ls->getMode() == 2 && spd.x > 0) {
                    m_collider.setLayer(terrain::TerrainLayer::NORMAL);
                } else if (ls->getMode() == 2 && spd.x < 0) {
                    m_collider.setLayer(terrain::TerrainLayer::ALTERNATE);
                }
            }
        }
        // STube
        if (ent.d_getType() == TYPE_STUBE_CNTRL) {
            GimGHZ_STube *_stube = (GimGHZ_STube *)it;
            if (d_collisionMain(*_stube)) {
                if (_stube->getMode() == 0) {
                    if (gsp >= 0 || spd.x >= 0)
                        sTube = true;
                    else if (gsp < 0 && spd.x < 0)
                        sTube = false;
                } else {
                    if (gsp >= 0 || spd.x >= 0)
                        sTube = false;
                    else if (gsp < 0 && spd.x < 0)
                        sTube = true;
                }

                if (m_stateMachine.currentId() != PlayerStateID::ROLL)
                    audio.dj().playSound(sndRoll_);
            }
        }
        // GHZ Bridge
        if (ent.d_getType() == TYPE_BRIDGE) {
            GimGHZ_Bridge *br = (GimGHZ_Bridge *)it;
            if ((!m_collider.isGrounded() && d_collisionBottom(*it)) ||
                (m_collider.isGrounded() &&
                 d_collisionBottomPlatform(*it, 14))) {
                br->setActive(true);
                dv_pos.y = ((*br).getY()) - ((*br).d_getHitBoxSize().y / 2) -
                           (dv_hitBoxSize.y / 2);

                if (!standOnObj) {
                    standOn = it;
                    standOnObj = true;
                    gsp = spd.x;
                }
            }
        }
        // GHZ Slope Platform
        if (ent.d_getType() == TYPE_GHZ_SLP_PLATFORM) {
            GimGHZ_SlpPlatform *slope = (GimGHZ_SlpPlatform *)it;
            if (((d_collisionBottom(*slope, 12) && m_collider.isGrounded()) ||
                 (d_collisionBottom(*slope, 1) && !m_collider.isGrounded())) &&
                spd.y >= 0 && dv_pos.y < slope->d_getPos().y - 30) {
                int _x = dv_pos.x - (slope->d_getPos().x -
                                     (slope->d_getHitBoxSize().x / 2));
                dv_pos.y = slope->d_getPos().y - (slope->getHeight(_x)) - 22 -
                           dv_hitBoxSize.y / 2;
                if (!standOnObj) {
                    standOnObj = true;
                    standOn = it;
                    gsp = spd.x;
                } else {
                    slope->d_destroy();

                    if (!slope->d_isLiving()) {
                        standOnObj = false;
                    }
                }

                if (!slope->d_isLiving()) {
                    audio.dj().playSound(sndDestroy_);
                }
            }
        }
        // Collision Solid
        if (ent.d_isSolid() && ent.d_isLiving()) {
            if (d_collisionRight(*it) && spd.x > 0.0) {
                dv_pos.x = ent.d_getPos().x - ent.d_getHitBoxSize().x / 2 -
                           dv_hitBoxSize.x / 2;
                spd.x = 0.0;
                gsp = 0.0;
            }
            if (d_collisionLeft(*it) && spd.x < 0.0) {
                dv_pos.x = ent.d_getPos().x + ent.d_getHitBoxSize().x / 2 +
                           dv_hitBoxSize.x / 2;
                spd.x = 0.0;
                gsp = 0.0;
            }

            if (d_collisionTop(*it) && spd.y < 0.0) {
                dv_pos.y = ent.d_getPos().y + ent.d_getHitBoxSize().y / 2 +
                           dv_hitBoxSize.y / 2;
                spd.y = 0.0;
                gsp = 0.0;
            }
        }
        // Platform
        if (ent.d_isPlatform() && ent.d_isLiving()) {
            if (((d_collisionBottomPlatform(*it, 12) &&
                  m_collider.isGrounded() && spd.y >= 0) ||
                 (d_collisionBottomPlatform(*it, 1) &&
                  !m_collider.isGrounded())) &&
                spd.y >= 0) {
                if (ent.d_isPlatPushUp())
                    dv_pos.y = ent.d_getPos().y - ent.d_getHitBoxSize().y / 2 -
                               dv_hitBoxSize.y / 2;

                if (!standOnObj) {
                    standOnObj = true;
                    standOn = it;

                    // if (action == ACT_ROLL)
                    // action = ACT_NORMAL;

                    gsp = spd.x;
                }

                if (ent.d_getType() == TYPE_PLATFORM) {
                    GimGHZ_Platform *plt = (GimGHZ_Platform *)it;

                    if (plt->isCanFall())
                        plt->setFalling(true);
                    if (plt->getDir() < 2)
                        dv_pos.x += plt->getSpd();
                }
            }
        }
    }

    if (standOn) {
        if (standOn->d_isLiving()) {
            if (!d_collisionBottomPlatform(*standOn, 12) || spd.y < 0) {
                standOnObj = false;
                standOn = nullptr;
            }
        } else {
            standOn = nullptr;
        }
    }
}

// === private === //
void Player::movement() {
    if (m_stateMachine.currentId() == PlayerStateID::DIE)
        return;

    if (dv_pos.x - 9 < 0 && spd.x < 0) {
        dv_pos.x = 9;
        gsp = 0;
        spd.x = 0;
    }

    if (m_collider.isGrounded()) {
        if ((m_collider.getMode() == PlayerSensorMode::FLOOR && gsp != 0) ||
            (m_collider.getMode() != PlayerSensorMode::FLOOR)) {
            if (m_stateMachine.currentId() != PlayerStateID::ROLL) {
                gsp -= PL_SLP * sinf(radians(angle));
            } else {
                if (sign(gsp) == sign(sinf(radians(angle))))
                    gsp -= PL_SLP_ROLL_UP * sinf(radians(angle));
                else
                    gsp -= PL_SLP_ROLL_DOWN * sinf(radians(angle));
            }
        }
        spd.x = gsp * cosf(radians(angle));
        spd.y = gsp * -sinf(radians(angle));
    }

    dv_pos.x += spd.x;
    dv_pos.y += spd.y;

    if (input.isKeyDebug() && !isDebugPressed) {
        debug = !debug;
        isDebugPressed = true;
    } else if (!input.isKeyDebug()) {
        isDebugPressed = false;
    }

    if (debug) {

        if (input.isKeyLeft())
            spd.x -= PL_AIR;
        else if (input.isKeyRight())
            spd.x += PL_AIR;
        else
            spd.x = 0;

        if (input.isKeyUp())
            spd.y -= PL_AIR;
        else if (input.isKeyDown())
            spd.y += PL_AIR;
        else
            spd.y = 0;

        return;
    }

    if (m_collider.isGrounded()) {
        if (m_stateMachine.currentId() != PlayerStateID::ROLL) {
            if (input.isKeyRight() && !input.isKeyLeft() && canHorMove) {
                if (gsp < 0.0)
                    gsp += PL_DEC;
                else if (gsp < PL_TOP)
                    gsp += PL_ACC;
            } else if (input.isKeyLeft() && !input.isKeyRight() && canHorMove) {
                if (gsp > 0.0)
                    gsp -= PL_DEC;
                else if (gsp > -PL_TOP)
                    gsp -= PL_ACC;
            } else {
                gsp -= fmin(fabs(gsp), PL_FRC) * float(fsign(gsp));
            }
        } else {
            if (input.isKeyRight() && !input.isKeyLeft() && canHorMove) {
                if (gsp < 0.0)
                    gsp += PL_DEC_ROLL;
            } else if (input.isKeyLeft() && !input.isKeyRight() && canHorMove) {
                if (gsp > 0.0)
                    gsp -= PL_DEC_ROLL;
            }
            gsp -= fmin(fabs(gsp), PL_FRC_ROLL) * float(fsign(gsp));
        }
    } else {
        if ((input.isKeyRight()) && (spd.x < PL_TOP) && canHorMove)
            spd.x += PL_AIR;
        else if ((input.isKeyLeft()) && (spd.x > -PL_TOP) && canHorMove)
            spd.x -= PL_AIR;
    }
}

void Player::gameplay() {
    if (debug)
        return;

    if (m_stateMachine.currentId() == PlayerStateID::DIE)
        return;

    // invincibility Timer
    if (invicTimer > 0 && m_stateMachine.currentId() != PlayerStateID::HURT)
        invicTimer--;
    if (ringTimer > 0)
        ringTimer--;

    // Hor lock timer
    if (horizontalLockTimer > -1) {
        // if (horizontalLockTimer == 0)
        //     canHorMove = true;
        // else
        //     canHorMove = false;

        horizontalLockTimer--;
    }

    // S Tube
    if (sTube) {
        m_stateMachine.changeTo(PlayerStateID::ROLL);

        if (gsp == 0.0) {
            gsp = (!animFlip) ? 2.0 : -2.0;
        }
    }

    // === Jump ===
    if (input.isKeyAction() && m_collider.isGrounded() && !sTube &&
        m_collider.isPlayerCanJump()) {
        m_stateMachine.changeTo(PlayerStateID::JUMP);
    }

    // Shift y pos and change m_collider.isGrounded() sensor width when player
    // is rolling
    if (m_stateMachine.isCurling()) {
        dv_hitBoxSize = v2f(20, 30);
        shiftY = 5;
    } else {
        dv_hitBoxSize = v2f(20, 40);
        shiftY = 0;
    }
}

void Player::animation() {
    // Animation angle
    if (m_collider.isGrounded()) {
        if (!m_stateMachine.isCurling() && (abs(gsp) > 0))
            animAngle = angle;
        else
            animAngle = 0.0;
    } else {
        if (animAngle != 0 &&
            m_stateMachine.currentId() != PlayerStateID::JUMP) {
            if (animAngle > 180.0)
                animAngle += 4.0;
            else
                animAngle -= 4.0;

            if (animAngle > 360.0)
                animAngle = 0.0;
            else if (animAngle < 0.0)
                animAngle = 0.0;
        } else {
            animAngle = 0;
        }
    }

    // Transform Angle
    diaAnim = false;
    if (animAngle > 337.5 || animAngle <= 22.5) {
        anim8Angle = 0;
    } else if (animAngle > 22.5 && animAngle <= 67.5) {
        if (!animFlip)
            anim8Angle = 0;
        else
            anim8Angle = 270;
        diaAnim = true;
    } else if (animAngle > 67.5 && animAngle <= 112.5) {
        anim8Angle = 270;
    } else if (animAngle > 112.5 && animAngle <= 157.5) {
        if (!animFlip)
            anim8Angle = 270;
        else
            anim8Angle = 180;
        diaAnim = true;
    } else if (animAngle > 157.5 && animAngle <= 202.5) {
        anim8Angle = 180;
    } else if (animAngle > 202.5 && animAngle <= 247.5) {
        if (!animFlip)
            anim8Angle = 180;
        else
            anim8Angle = 90;
        diaAnim = true;
    } else if (animAngle > 247.5 && animAngle <= 292.5) {
        anim8Angle = 90;
    } else if (animAngle > 292.5 && animAngle <= 337.5) {
        if (!animFlip)
            anim8Angle = 90;
        else
            anim8Angle = 0;
        diaAnim = true;
    }

    // Set sprite direction
    if (m_collider.isGrounded() &&
        m_stateMachine.currentId() != PlayerStateID::ROLL) {
        if ((gsp < 0.0) && (input.isKeyLeft()))
            animFlip = true;
        else if ((gsp > 0.0) && (input.isKeyRight()))
            animFlip = false;
    } else {
        if (input.isKeyLeft())
            animFlip = true;
        else if (input.isKeyRight())
            animFlip = false;
    }
}

void Player::getHit(std::list<Entity *> &entities) {
    if (m_stateMachine.currentId() == PlayerStateID::HURT || invicTimer > 0)
        return;

    if (rings != 0) {
        ringTimer = 64;
        m_collider.forceToAir();
        invicTimer = 120;

        m_stateMachine.changeTo(PlayerStateID::HURT);

        // Rings
        int t = 0;
        float _angle = 101.25f;
        bool _n = false;
        float _spd = 4.0;
        int _h = 1;
        while (t < rings && t < 32) {
            // TODO
            // Ring* r = new Ring(pos, *trn, float(cosf(radians(_angle)) * _spd
            // * _h),
            //     float(-sinf(radians(_angle)) * _spd));
            // r->create();
            // entities.push_back(r);
            _h *= -1;
            if (_n)
                _angle += 22.5f;
            _n = !_n;
            t++;
            if (t == 16) {
                _spd = 2;
                _angle = 101.25;
            }
        }
        rings = 0;

        audio.dj().playSound(sndRingLoss_);
    } else {
        m_stateMachine.changeTo(PlayerStateID::DIE);
    }
}
