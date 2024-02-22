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

class EntityHitBox {
public:
    EntityHitBox(v2f& position, v2i radius) : m_radius(radius), m_position(position) {}

    v2f getPosition() const { return m_position; }
    v2i getRadius() const { return m_radius; }

    float getLeft()   const { return m_position.x - (float)m_radius.x; }
    float getTop()    const { return m_position.y - (float)m_radius.y; }
    float getRight()  const { return m_position.x + (float)m_radius.x; }
    float getBottom() const { return m_position.y + (float)m_radius.y; }

    bool isOverlappingWith(const EntityHitBox& other) const {
        return (getLeft() < other.getRight()  && getRight() > other.getLeft() &&
                getTop()  > other.getBottom() && getBottom() < other.getTop());
    }
    
private:
    v2f& m_position;
    v2i  m_radius;
};

template<typename T>
class EntityComponent {
public: 
    EntityComponent(T* component) : m_component(component) {}

    bool exists() { return m_component != nullptr; }

    T& get() {
        if (m_component == nullptr)
            printf("Component does not exists");

        return *m_component;
    }

    static EntityComponent Empty() {
        return EntityComponent<T>(nullptr);
    }
private:
    T* m_component;
};

class EntitySolidBox {}; // TODO

enum class EntityTypeID {
    DEPRECATED,
    PLAYER,
    RING,
    SPRING,
    MONITOR,
    SIGN_POST,
    LAYER_SWITCH,
    ENEMY,
    EN_MOTOBUG,
    BRIDGE,
    BRIDGE_CNTRL,
    GHZ_SLP_PLATFORM,
    PLATFORM,
    BULLET,
    SPIKES,
    STUBE_CNTRL,
    PARTICLE,
};

struct EntityType {
    EntityType(EntityTypeID id) : id(id) {}

    EntityTypeID id;
    bool         isEnemy    = false;
    bool         isHarmful  = false;
};

#define DO_NOTHING_BY_DEFAULT
class Entity
{
    public:
        Entity() {}
        Entity(v2f _pos);

        // Basic methods
        virtual void init()             { DO_NOTHING_BY_DEFAULT }
        virtual void update()           { DO_NOTHING_BY_DEFAULT }
        virtual void draw(Camera& cam)  { DO_NOTHING_BY_DEFAULT }

        // Required
        virtual EntityType type() { return EntityType(EntityTypeID::DEPRECATED); }

        // Componenets ( Which can be optional by EntityComponent wrapper )
        virtual EntityComponent<v2f>            cposition()   { return EntityComponent<v2f>::Empty();            }
        virtual EntityComponent<v2f>            cspeed()      { return EntityComponent<v2f>::Empty();            }
        virtual EntityComponent<EntityHitBox>   chitbox()     { return EntityComponent<EntityHitBox>::Empty();   }
        virtual EntityComponent<EntitySolidBox> csolidBox()   { return EntityComponent<EntitySolidBox>::Empty(); }

        // Events
        virtual void onDestroy()   { DO_NOTHING_BY_DEFAULT }
        virtual void onOutOfView() { DO_NOTHING_BY_DEFAULT }

        // Deprecated
        virtual void d_update()           { dv_anim.tick(); }
        virtual void d_draw(Camera& cam)  { cam.draw(dv_anim, dv_pos); }
        virtual void d_reactingToOthers(std::list<Entity*>& entities) { return; }
        void         d_setPos(v2f pos) { this->dv_pos = pos; }
        v2f          d_getPos()         { return dv_pos; }
        void         d_goToStartPos()       { dv_pos = dv_startPos; }
        v2f          d_getStartPos()    { return dv_startPos; }
        void         d_setHitBoxSize(v2f _size)  { dv_hitBoxSize = _size; };
        v2f          d_getHitBoxSize()            { return dv_hitBoxSize;  };
        bool         d_entMeeting(Entity& ent, v2i meetSize);
        virtual void d_destroy() { dv_living = false; }
		uint8_t      d_getType()   { return dv_type; }
        bool         d_isSolid()      { return dv_solid; }
        bool         d_isPlatform()   { return dv_platform; }
        bool         d_isPlatPushUp() { return dv_platPushUp; }
        bool         d_isLiving()     { return dv_living; };
        bool         d_isInCamera(Camera& cam);
        bool         d_collisionRight(Entity& ent, uint32_t shiftY = 0);
        bool         d_collisionLeft(Entity& ent, uint32_t shiftY = 0);
        bool         d_collisionTop(Entity& ent, uint32_t shiftY = 0);
        bool         d_collisionBottom(Entity& ent, uint32_t shiftY = 0);
        bool         d_collisionBottomPlatform(Entity& ent, uint32_t shiftY = 0);
        bool         d_collisionMain(Entity& ent, uint32_t shiftY = 0);
    protected:
        // Deprecated variables
		uint8_t dv_type         = TYPE_UNKNOWN;
        bool    dv_solid        = false;
        bool    dv_platform     = false;
        bool    dv_platPushUp   = true;
        bool    dv_living       = true;
        v2f     dv_pos;
        v2f     dv_startPos;
        v2f     dv_hitBoxSize = v2f(16, 16);
        AnimMgr dv_anim;
};
#undef NOTHING

class EntityPool {
public:
    EntityPool(Camera& camera) : m_camera(camera) {}

    void add(Entity* entity) {
        m_pool.push_back(entity);
        entity->init();
    }

    void destroyEntity(Entity* entity) {
        m_pool.remove(entity);
        m_entitiesToDestroy.push_back(entity);
    }

    void update() {
        for (auto& it : m_pool) {
            if (!isEntityInsideView(*it)) continue;
            
            it->update();

            if (!isEntityInsideView(*it))
                it->onOutOfView();
        }

        for (auto& it : m_entitiesToDestroy) {
            it->onDestroy();
            delete it;
        }

        m_entitiesToDestroy.clear();
    }

    void draw() {
        for (auto& it : m_pool) {
            it->draw(m_camera);
        }
    }

private:    
    std::list<Entity*> m_pool;
    std::list<Entity*> m_entitiesToDestroy;

    Camera& m_camera;

private:
    bool isEntityInsideView(Entity& entity) {
        if (entity.type().id == EntityTypeID::DEPRECATED)
            return entity.d_isInCamera(m_camera);

        if (!entity.chitbox().exists())
            return true;

        const int DEACT_DELTA = 224;

        auto& hitbox = entity.chitbox().get();
        auto  viewPos = m_camera.getPos();

        return (hitbox.getLeft()   > viewPos.x - DEACT_DELTA) &&
               (hitbox.getRight()  < viewPos.x + DEACT_DELTA) && 
               (hitbox.getTop()    > viewPos.y - DEACT_DELTA) &&
               (hitbox.getBottom() < viewPos.y + DEACT_DELTA);
    }
};