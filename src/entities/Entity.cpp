#include "Entity.h"

Entity::Entity(v2f _pos)
{
    dv_pos = _pos;
    dv_startPos = _pos;
}

void Entity::update()
{
	dv_anim.tick();
}


bool Entity::d_collisionRight(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    for(int i = 4; i < dv_hitBoxSize.y-4; i++) {
        float xx = dv_pos.x + dv_hitBoxSize.x / 2 + 1;
        float yy = (dv_pos.y - dv_hitBoxSize.y / 2) + i + shiftY;
        if ((yy > ent.dv_pos.y - ent.dv_hitBoxSize.y / 2) && (yy < ent.dv_pos.y + ent.dv_hitBoxSize.y / 2) &&
            (xx > ent.dv_pos.x - ent.dv_hitBoxSize.x / 2) && (xx < ent.dv_pos.x + ent.dv_hitBoxSize.x / 2))
                return true;
    }
    return false;
}

bool Entity::d_collisionLeft(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    for(int i = 4; i < dv_hitBoxSize.y-4; i++) {
        float xx = dv_pos.x - dv_hitBoxSize.x / 2 - 1;
        float yy = (dv_pos.y - dv_hitBoxSize.y / 2) + i + shiftY;
        if ((yy > ent.dv_pos.y - ent.dv_hitBoxSize.y / 2) && (yy < ent.dv_pos.y + ent.dv_hitBoxSize.y / 2) &&
            (xx > ent.dv_pos.x - ent.dv_hitBoxSize.x / 2) && (xx < ent.dv_pos.x + ent.dv_hitBoxSize.x / 2))
                return true;
    }
    return false;
}

bool Entity::d_collisionTop(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    for(int i = 0; i < dv_hitBoxSize.x - 8; i++) {
        float xx = (dv_pos.x - dv_hitBoxSize.x / 2) + i;
        float yy = dv_pos.y - dv_hitBoxSize.y / 2 + shiftY;
        if ((yy > ent.dv_pos.y - ent.dv_hitBoxSize.y / 2) && (yy < ent.dv_pos.y + ent.dv_hitBoxSize.y / 2) &&
            (xx > ent.dv_pos.x - ent.dv_hitBoxSize.x / 2) && (xx < ent.dv_pos.x + ent.dv_hitBoxSize.x / 2))
                return true;
    }
    return false;
}

bool Entity::d_collisionBottom(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    for(int i = 2; i < dv_hitBoxSize.x - 2; i++) {
        float xx = (dv_pos.x - dv_hitBoxSize.x / 2) + i;
        float yy = dv_pos.y + dv_hitBoxSize.y / 2 + shiftY;
        if ((yy > ent.dv_pos.y - ent.dv_hitBoxSize.y / 2) && (yy < ent.dv_pos.y + ent.dv_hitBoxSize.y / 2) &&
            (xx > ent.dv_pos.x - ent.dv_hitBoxSize.x / 2) && (xx < ent.dv_pos.x + ent.dv_hitBoxSize.x / 2))
                return true;
    }
    return false;
}


bool Entity::d_collisionBottomPlatform(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    for(int i = 2; i < dv_hitBoxSize.x - 2; i++) {
        float xx = (dv_pos.x - dv_hitBoxSize.x / 2) + i;
        for (int j = 0; j < shiftY; j++) {
            float yy = dv_pos.y + dv_hitBoxSize.y / 2 + j;
            if ((yy > ent.dv_pos.y - ent.dv_hitBoxSize.y / 2) && (yy < ent.dv_pos.y + ent.dv_hitBoxSize.y / 2) &&
                (xx > ent.dv_pos.x - ent.dv_hitBoxSize.x / 2) && (xx < ent.dv_pos.x + ent.dv_hitBoxSize.x / 2))
                    return true;
        }
    }
    return false;
}

bool Entity::d_collisionMain(Entity& ent, uint32_t shiftY)
{
    if (!dv_living) return false;

    if (d_collisionLeft(ent, shiftY) || d_collisionRight(ent, shiftY) || 
		d_collisionTop(ent, shiftY) || d_collisionBottom(ent, shiftY))
        return true;
    else
        return false;
}

void Entity::draw(Camera& cam) {
    cam.draw(dv_anim, dv_pos);
}


bool Entity::d_isInCamera(Camera& cam)
{
    float rectR, rectL, rectT, rectB;
    #define DEACT_DELTA  224

    rectR = (dv_pos.x + dv_hitBoxSize.x / 2);
    rectL = (dv_pos.x - dv_hitBoxSize.x / 2);
    rectT = (dv_pos.y - dv_hitBoxSize.y / 2);
    rectB = (dv_pos.y + dv_hitBoxSize.y / 2);

    if ( (dv_startPos.x >= cam.getPos().x - DEACT_DELTA) && (dv_startPos.x <= cam.getPos().x + cam.getSize().width + DEACT_DELTA) &&
         (dv_startPos.y >= cam.getPos().y - DEACT_DELTA) && (dv_startPos.y <= cam.getPos().y + cam.getSize().height + DEACT_DELTA))
        return true;

    if ( (rectR < cam.getPos().x - DEACT_DELTA) || (rectL > cam.getPos().x + cam.getSize().width + DEACT_DELTA) ||
         (rectB < cam.getPos().y - DEACT_DELTA) || (rectT > cam.getPos().y + cam.getSize().height + DEACT_DELTA))
        return false;
    else
        return true;
}

bool Entity::d_entMeeting(Entity& ent, v2i meetSize)
{
    float enRectR, enRectL, enRectT, enRectB;

    enRectR = (ent.d_getPos().x + dv_hitBoxSize.x / 2);
    enRectL = (ent.d_getPos().x - dv_hitBoxSize.x / 2);
    enRectT = (ent.d_getPos().y - dv_hitBoxSize.y / 2);
    enRectB = (ent.d_getPos().y + dv_hitBoxSize.y / 2);

    if ( (enRectR < dv_pos.x - meetSize.x) || (enRectL > dv_pos.x + meetSize.x) ||
         (enRectB < dv_pos.y - meetSize.y) || (enRectT > dv_pos.y + meetSize.y))
        return false;
    else
        return true;
}

