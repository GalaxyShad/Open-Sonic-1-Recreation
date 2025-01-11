#include "GimmicksGHZ.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include <cstddef>

void GimGHZ_Stone::init() 
{
    dv_hitBoxSize = v2f(32.0, 32.0);
    dv_type = TYPE_UNKNOWN;
	// dv_anim.create(TEX_GHZ_GIMM);
    // dv_anim.set(0, 0, 0);
    dv_solid = true;
    dv_platform = true;
}

GimGHZ_BridgeController::GimGHZ_BridgeController(v2f _pos, artist_api::Sprite &spr, uint8_t count, std::list<Entity*>& ent) : Entity(_pos), spr_(spr)
{
    segmentsCount = count;
    segments = new GimGHZ_Bridge*[count];
	for(int i = -count / 2; i < count / 2; i++) {
        GimGHZ_Bridge* segment = new GimGHZ_Bridge(v2f(_pos.x + i * 16, _pos.y),spr_);
        
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

void GimGHZ_BridgeController::init() 
{
    dv_hitBoxSize = v2f(16.0, 16.0);
    dv_type = TYPE_UNKNOWN;
}

void GimGHZ_BridgeController::d_update() 
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
        xx = (*segments[i]).d_getStartPos().x;
        yy = (*segments[i]).d_getStartPos().y;

        float logY = yy + ((*segments[curSegment]).maxDepression * sinf(radians(float(angle) * (1.0 - LogDistance))));
        (*segments[i]).d_setPos(v2f(xx, logY));
    }
}

void GimGHZ_Bridge::init() 
{
    dv_hitBoxSize = v2f(16.0, 16.0);
    dv_type = TYPE_BRIDGE;
	// dv_anim.create(TEX_GHZ_GIMM);
    // dv_anim.set(2, 2, 0);
    dv_platform = true;
    dv_platPushUp = false;
}

float GimGHZ_Bridge::getY() 
{
     
    float LogDistance = 1.0 / float((count - number) + 1);
    return dv_startPos.y + float(maxDepression * sinf(radians(90.0 * (1.0 - LogDistance))));
}

void GimGHZ_SlpPlatform::init() 
{
    deathTimer = -1;
    dv_hitBoxSize = v2f(96.0, 84.0);
    dv_type = TYPE_GHZ_SLP_PLATFORM;
	// dv_anim.create(TEX_GHZ_GIMM);
    // dv_anim.set(1, 1, 0);
    dv_platPushUp = false;
}

void GimGHZ_SlpPlatform::d_update() { 
    if (deathTimer != -1) {
        if (deathTimer != 0) {
            deathTimer--;
        } else {
            dv_living = false;
            v2f _p = dv_pos;

            if (!isLeft) {
                _p.x -= 48; _p.y -= 56;
                for (int i = 0; i < 36; i++){
                    v2f offset = v2f((i%6)*16, (i/6)*16);
                    artist_api::Sprite part_spr = spr_;
                    part_spr.rect = {48+offset.x,  0+offset.y,  16, 16};
                    ent.push_front(new GimGHZ_SlpPlatformPart(v2f(_p.x+offset.x, _p.y+offset.y),part_spr, i, false));
                }

            } else {
                _p.x += 32; _p.y -= 56;
                for (int i = 0; i < 36; i++){
                    v2f offset = v2f((i%6)*16, (i/6)*16);
                    artist_api::Sprite part_spr = spr_;
                    part_spr.rect = {48+offset.x,  0+offset.y,  16, 16};
                    ent.push_front(new GimGHZ_SlpPlatformPart(v2f(_p.x-offset.x, _p.y+offset.y),part_spr, i, true));
                }
            }
        }
    }
}

void GimGHZ_Platform::init() 
{
    dv_hitBoxSize = v2f(64.0, 26.0);
    dv_type = TYPE_PLATFORM;
	// dv_anim.create(TEX_GHZ_GIMM);
    // dv_anim.set(3, 3, 0);
    dv_platform = true;
    angle = 0;

    xsp = 0;
    ysp = 0;

    falling = false;
    fallTimer = -1;
}

void GimGHZ_Platform::d_update() 
{
    if (isFalling()) {
        if (fallTimer > 0) {
            fallTimer--;
        } else {
            dv_platform = false;
            ysp += 0.09375;
            dv_pos.y += ysp;
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

    dv_pos.x += xsp;
    dv_pos.y += ysp;

    angle += 1;
    if (angle >= 360)
        angle = 0;
}
