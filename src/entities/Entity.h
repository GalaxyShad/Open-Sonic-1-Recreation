#pragma once

#include <list>

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_GHZ_TILES 2

#include "../FramesMappings.h"
#include "Camera.h"

#include "../core/_index.hpp"

enum Types {
    TYPE_UNKNOWN,
    TYPE_PLAYER,
    TYPE_RING,
    TYPE_SPRING,
    TYPE_MONITOR,
    TYPE_SIGN_POST,
    TYPE_LAYER_SWITCH,
    TYPE_ENEMY,
    TYPE_EN_MOTOBUG,
    TYPE_BRIDGE,
    TYPE_BRIDGE_CNTRL,
    TYPE_GHZ_SLP_PLATFORM,
    TYPE_PLATFORM,
    TYPE_BULLET,
    TYPE_SPIKES,
    TYPE_STUBE_CNTRL,
    TYPE_PARTICLE
};

class Entity
{
    public:
        Entity() {}
        Entity(v2f _pos);
        virtual void init() { return; }
        virtual void update();
        virtual void draw(Camera& cam);
        virtual void d_reactingToOthers(std::list<Entity*>& entities) { return; }

        void d_setPos(v2f pos) { this->dv_pos = pos; }
        v2f d_getPos()         { return dv_pos; }

        void d_goToStartPos()       { dv_pos = dv_startPos; }
        v2f d_getStartPos()    { return dv_startPos; }
        
        void d_setHitBoxSize(v2f _size)  { dv_hitBoxSize = _size; };
        v2f d_getHitBoxSize()            { return dv_hitBoxSize;  };

        bool d_entMeeting(Entity& ent, v2i meetSize);

        virtual void d_destroy() { dv_living = false; }

		uint8_t d_getType()   { return dv_type; }

        bool d_isSolid()      { return dv_solid; }
        bool d_isPlatform()   { return dv_platform; }
        bool d_isPlatPushUp() { return dv_platPushUp; }
        bool d_isLiving()     { return dv_living; };

        bool d_isInCamera(Camera& cam);
        
        bool d_collisionRight(Entity& ent, uint32_t shiftY = 0);
        bool d_collisionLeft(Entity& ent, uint32_t shiftY = 0);
        bool d_collisionTop(Entity& ent, uint32_t shiftY = 0);
        bool d_collisionBottom(Entity& ent, uint32_t shiftY = 0);
        bool d_collisionBottomPlatform(Entity& ent, uint32_t shiftY = 0);
        bool d_collisionMain(Entity& ent, uint32_t shiftY = 0);
    protected:
		uint8_t dv_type = TYPE_UNKNOWN;
        bool dv_solid = false;
        bool dv_platform = false;
        bool dv_platPushUp = true;
        bool dv_living = true;
        v2f dv_pos;
        v2f dv_startPos;
        v2f dv_hitBoxSize = v2f(16, 16);
        AnimMgr dv_anim;
};