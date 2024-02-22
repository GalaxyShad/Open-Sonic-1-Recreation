#include "EnemiesGHZ.h"

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

void Bullet::update()
{
    if (dv_anim.getCurFrame() >= 127)
        dv_anim.set(126, 127, 0.2f);

    dv_pos.x += xsp;
    dv_pos.y += ysp;

    if (mode != 0) 
        ysp += grv;

    dv_anim.tick();
}

void Bullet::draw(Camera &cam)
{
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(dv_anim, dv_pos, 0.0, flip);
}

// Motobug
void EnMotobug::init()
{
    dv_hitBoxSize = v2f(42, 34);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(134, 137, 0.2f);
}

void EnMotobug::update()
{
    dv_pos.x += MOTOBUG_SPD * dir;
    dv_anim.tick();
}

void EnMotobug::draw(Camera &cam)
{
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(dv_anim, dv_pos, 0.0, flip);
}

void EnMotobug::trnCollision(Terrain &trn)
{
    // Ground collision
    for (int i = 0; i < 30; i++)
    {
        v2i sensorPos = v2i(dv_pos.x, dv_pos.y + i);
        if ((trn.getTileVerHeight(sensorPos) != 0) &&
                (trn.getTileType(sensorPos) != TILE_LBR) &&
                (trn.getTileType(sensorPos) != TILE_EMPTY))
        {
            int tilePosY = int((dv_pos.y + i) / 16) * 16;
            dv_pos.y = tilePosY + 16 - trn.getTileVerHeight(sensorPos) - 14;
            break;
        }
    }

    v2i lSensorPos = v2i(dv_pos.x - 17, dv_pos.y + 20);
    v2i rSensorPos = v2i(dv_pos.x + 17, dv_pos.y + 20);
    if ((trn.getTileType(lSensorPos) == TILE_EMPTY && dir == -1) ||
            (trn.getTileType(rSensorPos) == TILE_EMPTY && dir == 1))
    {
        dir = -dir;
    }
}

// Crab
void EnCrab::init()
{
    dv_hitBoxSize = v2f(42, 30);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(113, 115, 0.2, true);
}

void EnCrab::update()
{
    if (moveTimer == 0)
    {
        dv_anim.set(113, 115, 0.1, true);
        if (faceRight)
            xsp = CRAB_SPD;
        else
            xsp = -CRAB_SPD;
    }
    else
    {
        moveTimer--;
        xsp = 0.0;
        if (moveTimer >= 25)
        {
            dv_anim.set(113, 113, 0);
            bulFlag = false;
        }
        else
        {
            dv_anim.set(116, 116, 0);
            if (!bulFlag)
            {
                entities->push_back(new Bullet(v2f(dv_pos.x - 18, dv_pos.y - 8), 1, -1));
                entities->push_back(new Bullet(v2f(dv_pos.x + 18, dv_pos.y - 8), 1, 1));
                bulFlag = true;
            }
        }
    }

    if (dv_pos.x > dv_startPos.x + 72)
    {
        moveTimer = 50;
        faceRight = false;
        xsp *= -1;
    }
    if (dv_pos.x < dv_startPos.x - 72)
    {
        moveTimer = 50;
        faceRight = true;
        xsp *= -1;
    }

    dv_pos.x += xsp;
    dv_anim.tick();
}

void EnCrab::trnCollision(Terrain &trn)
{
    v2i lSensorPos = v2i(dv_pos.x - 17, dv_pos.y);
    v2i rSensorPos = v2i(dv_pos.x + 17, dv_pos.y);

    // Wall collision
    if ((trn.getTileHorHeight(lSensorPos) != 0) &&
            (trn.getTileType(lSensorPos) != TILE_EMPTY) && xsp < 0.0)
    {
        moveTimer = 50;
        faceRight = true;
        xsp *= -1;
    }
    else if ((trn.getTileHorHeight(rSensorPos) != 0) &&
                     (trn.getTileType(rSensorPos) != TILE_EMPTY) && xsp > 0.0)
    {
        moveTimer = 50;
        faceRight = false;
        xsp *= -1;
    }

    // Turn around if no ground
    lSensorPos = v2i(dv_pos.x - 17, dv_pos.y + 24);
    rSensorPos = v2i(dv_pos.x + 17, dv_pos.y + 24);
    if ((trn.getTileType(lSensorPos) == TILE_EMPTY && xsp < 0.0) ||
            (trn.getTileType(rSensorPos) == TILE_EMPTY && xsp > 0.0))
    {
        xsp *= -1;
        faceRight = !faceRight;
        moveTimer = 50;
    }

    // Ground Collision
    for (int i = 0; i < 50; i++)
    {
        lSensorPos = v2i(dv_pos.x - 15, dv_pos.y + i);
        rSensorPos = v2i(dv_pos.x + 15, dv_pos.y + i);
        if (((trn.getTileVerHeight(lSensorPos) != 0) &&
                 (trn.getTileType(lSensorPos) != TILE_LBR) &&
                 (trn.getTileType(lSensorPos) != TILE_EMPTY)) ||
                ((trn.getTileVerHeight(rSensorPos) != 0) &&
                 (trn.getTileType(rSensorPos) != TILE_LBR) &&
                 (trn.getTileType(rSensorPos) != TILE_EMPTY)))
        {

            int tilePosY = int((dv_pos.y + i) / 16) * 16;

            int height = std::max(trn.getTileVerHeight(lSensorPos), trn.getTileVerHeight(rSensorPos));
            dv_pos.y = tilePosY + 16 - height - 15;
            break;
        }
    }
}

// BUZZ
void EnBuzz::init()
{
    dv_hitBoxSize = v2f(48, 24);
    moveTimer = 192;
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    animWings.create(TEX_OBJECTS);
    animWings.set(119, 120, 0.75);
    dv_anim.set(117, 117, 0);
}

void EnBuzz::update()
{
    if (moveTimer <= 0)
    {
        if (idleTimer > 0)
        {
            idleTimer--;
            xsp = 0.0;
            if (idleTimer <= 34 && fired)
            {
                dv_anim.set(118, 118, 0);
            }
        }
        else
        {
            dv_anim.set(117, 117, 0);
            if (moveTimer == 0)
                fired = false;
            if (!fired)
            {
                if (faceRight)
                    faceRight = false;
                else
                    faceRight = true;
            }
            moveTimer = 128;
        }
    }
    else
    {
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

void EnBuzz::d_reactingToOthers(std::list<Entity *> &entities)
{
    // Player Detection
    std::list<Entity *>::iterator it;
    for (it = entities.begin(); it != entities.end(); it++)
    {
        if ((*it)->d_getType() == TYPE_PLAYER)
        {
            if (d_entMeeting(*(*it), v2i(48, 96)) && !fired)
            {
                moveTimer = -1;
                idleTimer = 48;

                fired = true;
            }
            break;
        }
    }
    if (fired && idleTimer == 24)
    {
        if (!faceRight)
            entities.push_back(new Bullet(v2f(dv_pos.x - 17, dv_pos.y + 23), 0, -1));
        else
            entities.push_back(new Bullet(v2f(dv_pos.x + 17, dv_pos.y + 23), 0, 1));
    }
}

void EnBuzz::draw(Camera &cam)
{
    v2f wPos;

    if (faceRight)
    {
        if (int(dv_anim.getCurFrame()) == 118)
            wPos = v2f(dv_pos.x - 2, dv_pos.y - 12);
        else
            wPos = v2f(dv_pos.x + 4, dv_pos.y - 9);
    }
    else
    {
        if (int(dv_anim.getCurFrame()) == 118)
            wPos = v2f(dv_pos.x + 2, dv_pos.y - 12);
        else
            wPos = v2f(dv_pos.x - 4, dv_pos.y - 9);
    }

    cam.draw(animWings, wPos, 0.0, faceRight);
    cam.draw(dv_anim, dv_pos, 0.0, faceRight);
}

// Chopper
void EnChopper::init()
{
    dv_hitBoxSize = v2f(26, 34);
    dv_type = TYPE_ENEMY;
    dv_anim.create(TEX_OBJECTS);
    dv_anim.set(111, 112, 0.2f);
}

void EnChopper::update()
{
    dv_pos.y += ysp;

    if (dv_pos.y >= dv_startPos.y - 24)
        ysp = -7;
    ysp += 0.09375;

    dv_anim.tick();
}