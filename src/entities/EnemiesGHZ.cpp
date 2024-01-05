#include "EnemiesGHZ.h"

// Bullet
void Bullet::create() {
    hitBoxSize = Vector2f(14, 14);
    type = TYPE_BULLET;
    anim.create(TEX_OBJECTS);
    if (mode == 0) {
        anim.set(123, 127, 0.2f);
        xsp = 2 * dir;
        ysp = 2;
    } else {
        anim.set(126, 127, 0.2f);
        ysp = -4;
        xsp = 1 * dir;
    }
}

void Bullet::update()
{
    if (anim.getCurFrame() >= 127)
        anim.set(126, 127, 0.2f);

    pos.x += xsp;
    pos.y += ysp;

    if (mode != 0) 
        ysp += grv;

    anim.tick();
}

void Bullet::draw(Camera &cam)
{
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(anim, pos, 0.0, flip);
}

// Motobug
void EnMotobug::create()
{
    hitBoxSize = Vector2f(42, 34);
    type = TYPE_ENEMY;
    anim.create(TEX_OBJECTS);
    anim.set(134, 137, 0.2f);
}

void EnMotobug::update()
{
    pos.x += MOTOBUG_SPD * dir;
    anim.tick();
}

void EnMotobug::draw(Camera &cam)
{
    bool flip = false;
    if (dir == 1)
        flip = true;

    cam.draw(anim, pos, 0.0, flip);
}

void EnMotobug::trnCollision(Terrain &trn)
{
    // Ground collision
    for (int i = 0; i < 30; i++)
    {
        Vector2i sensorPos = Vector2i(pos.x, pos.y + i);
        if ((trn.getTileVerHeight(sensorPos) != 0) &&
                (trn.getTileType(sensorPos) != TILE_LBR) &&
                (trn.getTileType(sensorPos) != TILE_EMPTY))
        {
            int tilePosY = int((pos.y + i) / 16) * 16;
            pos.y = tilePosY + 16 - trn.getTileVerHeight(sensorPos) - 14;
            break;
        }
    }

    Vector2i lSensorPos = Vector2i(pos.x - 17, pos.y + 20);
    Vector2i rSensorPos = Vector2i(pos.x + 17, pos.y + 20);
    if ((trn.getTileType(lSensorPos) == TILE_EMPTY && dir == -1) ||
            (trn.getTileType(rSensorPos) == TILE_EMPTY && dir == 1))
    {
        dir = -dir;
    }
}

// Crab
void EnCrab::create()
{
    hitBoxSize = Vector2f(42, 30);
    type = TYPE_ENEMY;
    anim.create(TEX_OBJECTS);
    anim.set(113, 115, 0.2, true);
}

void EnCrab::update()
{
    if (moveTimer == 0)
    {
        anim.set(113, 115, 0.1, true);
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
            anim.set(113, 113, 0);
            bulFlag = false;
        }
        else
        {
            anim.set(116, 116, 0);
            if (!bulFlag)
            {
                entities->push_back(new Bullet(Vector2f(pos.x - 18, pos.y - 8), 1, -1));
                entities->push_back(new Bullet(Vector2f(pos.x + 18, pos.y - 8), 1, 1));
                bulFlag = true;
            }
        }
    }

    if (pos.x > startPos.x + 72)
    {
        moveTimer = 50;
        faceRight = false;
        xsp *= -1;
    }
    if (pos.x < startPos.x - 72)
    {
        moveTimer = 50;
        faceRight = true;
        xsp *= -1;
    }

    pos.x += xsp;
    anim.tick();
}

void EnCrab::trnCollision(Terrain &trn)
{
    Vector2i lSensorPos = Vector2i(pos.x - 17, pos.y);
    Vector2i rSensorPos = Vector2i(pos.x + 17, pos.y);

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
    lSensorPos = Vector2i(pos.x - 17, pos.y + 24);
    rSensorPos = Vector2i(pos.x + 17, pos.y + 24);
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
        lSensorPos = Vector2i(pos.x - 15, pos.y + i);
        rSensorPos = Vector2i(pos.x + 15, pos.y + i);
        if (((trn.getTileVerHeight(lSensorPos) != 0) &&
                 (trn.getTileType(lSensorPos) != TILE_LBR) &&
                 (trn.getTileType(lSensorPos) != TILE_EMPTY)) ||
                ((trn.getTileVerHeight(rSensorPos) != 0) &&
                 (trn.getTileType(rSensorPos) != TILE_LBR) &&
                 (trn.getTileType(rSensorPos) != TILE_EMPTY)))
        {

            int tilePosY = int((pos.y + i) / 16) * 16;

            int height = std::max(trn.getTileVerHeight(lSensorPos), trn.getTileVerHeight(rSensorPos));
            pos.y = tilePosY + 16 - height - 15;
            break;
        }
    }
}

// BUZZ
void EnBuzz::create()
{
    hitBoxSize = Vector2f(48, 24);
    moveTimer = 192;
    type = TYPE_ENEMY;
    anim.create(TEX_OBJECTS);
    animWings.create(TEX_OBJECTS);
    animWings.set(119, 120, 0.75);
    anim.set(117, 117, 0);
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
                anim.set(118, 118, 0);
            }
        }
        else
        {
            anim.set(117, 117, 0);
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

    pos.x += xsp;
    anim.tick();
    animWings.tick();
}

void EnBuzz::reactingToOthers(std::list<Entity *> &entities)
{
    // Player Detection
    std::list<Entity *>::iterator it;
    for (it = entities.begin(); it != entities.end(); it++)
    {
        if ((*it)->getType() == TYPE_PLAYER)
        {
            if (entMeeting(*(*it), Vector2i(48, 96)) && !fired)
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
            entities.push_back(new Bullet(Vector2f(pos.x - 17, pos.y + 23), 0, -1));
        else
            entities.push_back(new Bullet(Vector2f(pos.x + 17, pos.y + 23), 0, 1));
    }
}

void EnBuzz::draw(Camera &cam)
{
    Vector2f wPos;

    if (faceRight)
    {
        if (int(anim.getCurFrame()) == 118)
            wPos = Vector2f(pos.x - 2, pos.y - 12);
        else
            wPos = Vector2f(pos.x + 4, pos.y - 9);
    }
    else
    {
        if (int(anim.getCurFrame()) == 118)
            wPos = Vector2f(pos.x + 2, pos.y - 12);
        else
            wPos = Vector2f(pos.x - 4, pos.y - 9);
    }

    cam.draw(animWings, wPos, 0.0, faceRight);
    cam.draw(anim, pos, 0.0, faceRight);
}

// Chopper
void EnChopper::create()
{
    hitBoxSize = Vector2f(26, 34);
    type = TYPE_ENEMY;
    anim.create(TEX_OBJECTS);
    anim.set(111, 112, 0.2f);
}

void EnChopper::update()
{
    pos.y += ysp;

    if (pos.y >= startPos.y - 24)
        ysp = -7;
    ysp += 0.09375;

    anim.tick();
}