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
        virtual void create() { return; }
        virtual void update();
        virtual void draw(Camera& cam);
        virtual void reactingToOthers(std::list<Entity*>& entities) { return; }

        void setPos(v2f pos) { this->pos = pos; }
        v2f getPos()         { return pos; }

        void goToStartPos()       { pos = startPos; }
        v2f getStartPos()    { return startPos; }
        
        void setHitBoxSize(v2f _size)  { hitBoxSize = _size; };
        v2f getHitBoxSize()            { return hitBoxSize;  };

        bool entMeeting(Entity& ent, v2i meetSize);

        virtual void destroy() { living = false; }

		uint8_t getType()   { return type; }

        bool isSolid()      { return solid; }
        bool isPlatform()   { return platform; }
        bool isPlatPushUp() { return platPushUp; }
        bool isLiving()     { return living; };

        bool isInCamera(Camera& cam);
        
        bool collisionRight(Entity& ent, uint32_t shiftY = 0);
        bool collisionLeft(Entity& ent, uint32_t shiftY = 0);
        bool collisionTop(Entity& ent, uint32_t shiftY = 0);
        bool collisionBottom(Entity& ent, uint32_t shiftY = 0);
        bool collisionBottomPlatform(Entity& ent, uint32_t shiftY = 0);
        bool collisionMain(Entity& ent, uint32_t shiftY = 0);
    protected:
		uint8_t type = TYPE_UNKNOWN;
        bool solid = false;
        bool platform = false;
        bool platPushUp = true;
        bool living = true;
        v2f pos;
        v2f startPos;
        v2f hitBoxSize = v2f(16, 16);
        AnimMgr anim;
};