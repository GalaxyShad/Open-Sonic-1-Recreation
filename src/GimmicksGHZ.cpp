#include "GimmicksGHZ.h"
#include "Geometry.h"

void GimGHZ_Stone::create() 
{
    hitBoxSize = Vector2f(32.0, 32.0);
    type = TYPE_UNKNOWN;
	anim.create(TEX_GHZ_GIMM);
    anim.set(0, 0, 0);
    solid = true;
    platform = true;
}

GimGHZ_BridgeController::GimGHZ_BridgeController(Vector2f _pos, uint8_t count, std::list<Entity*>& ent) : Entity(_pos)
{
    segmentsCount = count;
    segments = new GimGHZ_Bridge*[count];
	for(int i = -count / 2; i < count / 2; i++) {
        GimGHZ_Bridge* segment = new GimGHZ_Bridge(Vector2f(_pos.x + i * 16, _pos.y));
        
        if (i < 0)
            segment->maxDepression = ((count / 2) + i + 1) * 2;
        else 
            segment->maxDepression = ((count / 2) - i) * 2;

        segment->count = segmentsCount;
        segment->number = count / 2 + i;

        segments[count / 2 + i] = segment;
		ent.push_back(segment);
    }
}

void GimGHZ_BridgeController::create() 
{
    hitBoxSize = Vector2f(16.0, 16.0);
    type = TYPE_UNKNOWN;
}

void GimGHZ_BridgeController::update() 
{
    float LogDistance = 0;
    bool isActiveExist = false;
    for (int i = 0; i < segmentsCount; i++)
    {
        if ((*segments[i]).isActive()) {
            curSegment = i;
            isActiveExist = true;
            angle = 90;
        }
        (*segments[i]).setActive(false);
    }
    if (!isActiveExist && angle > 0)
        angle -= 5;

    for (int i = 0; i < segmentsCount; i++) {
            
        float log_difference = fabs((i+1) - float(curSegment));
            
        if (i < curSegment) 
            LogDistance = log_difference / float(curSegment); 
        else
            LogDistance = log_difference / float((segmentsCount - curSegment) + 1);
            
        float xx, yy;
        xx = (*segments[i]).getStartPos().x;
        yy = (*segments[i]).getStartPos().y;

        float logY = yy + ((*segments[curSegment]).maxDepression * sinf(radians(float(angle) * (1.0 - LogDistance))));
        (*segments[i]).setPos(Vector2f(xx, logY));
    }
}

void GimGHZ_Bridge::create() 
{
    hitBoxSize = Vector2f(16.0, 16.0);
    type = TYPE_BRIDGE;
	anim.create(TEX_GHZ_GIMM);
    anim.set(2, 2, 0);
    platform = true;
    platPushUp = false;
}

float GimGHZ_Bridge::getY() 
{
     
    float LogDistance = 1.0 / float((count - number) + 1);
    return startPos.y + float(maxDepression * sinf(radians(90.0 * (1.0 - LogDistance))));
}

void GimGHZ_SlpPlatform::create() 
{
    deathTimer = -1;
    hitBoxSize = Vector2f(96.0, 84.0);
    type = TYPE_GHZ_SLP_PLATFORM;
	anim.create(TEX_GHZ_GIMM);
    anim.set(1, 1, 0);
    platPushUp = false;
}

void GimGHZ_SlpPlatform::update() { 
    if (deathTimer != -1) {
        if (deathTimer != 0) {
            deathTimer--;
        } else {
            living = false;
            Vector2f _p = pos;

            if (!isLeft) {
                _p.x -= 48; _p.y -= 56;
                for (int i = 0; i < 36; i++) 
                    ent.push_front(new GimGHZ_SlpPlatformPart(
                        Vector2f(_p.x+(i%6)*16, _p.y+(i/6)*16), i, false));
            } else {
                _p.x += 32; _p.y -= 56;
                for (int i = 0; i < 36; i++) 
                    ent.push_front(new GimGHZ_SlpPlatformPart(
                        Vector2f(_p.x-(i%6)*16, _p.y+(i/6)*16), i, true));
            }
        }
    }
}

void GimGHZ_Platform::create() 
{
    hitBoxSize = Vector2f(64.0, 26.0);
    type = TYPE_PLATFORM;
	anim.create(TEX_GHZ_GIMM);
    anim.set(3, 3, 0);
    platform = true;
    angle = 0;

    xsp = 0;
    ysp = 0;

    falling = false;
    fallTimer = -1;
}

void GimGHZ_Platform::update() 
{
    if (isFalling()) {
        if (fallTimer > 0) {
            fallTimer--;
        } else {
            platform = false;
            ysp += 0.09375;
            pos.y += ysp;
        }
    }

    if (!mooving) return;

    switch (dir)
    {
        case 0: xsp = cosf(radians(angle)) * spd; break;
        case 1: xsp = -cosf(radians(angle)) * spd; break;
        case 2: ysp = -cosf(radians(angle)) * spd; break;
        case 3: ysp = cosf(radians(angle)) * spd; break;
    }

    pos.x += xsp;
    pos.y += ysp;

    angle += 1;
    if (angle >= 360)
        angle = 0;
}
