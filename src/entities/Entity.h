#pragma once

#include "core/game_enviroment/artist/ArtistStructs.h"
#include <list>
#include <vector>

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_GHZ_TILES 2

#include "../FramesMappings.h"
#include "Camera.h"
#include "entity-types.hpp"

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
    EntityHitBox(v2f &position, v2i radius)
        : m_radius(radius), m_position(position) {}

    v2f getPosition() const { return m_position; }
    v2i getRadius() const { return m_radius; }

    float getLeft() const { return m_position.x - (float)m_radius.x; }
    float getTop() const { return m_position.y - (float)m_radius.y; }
    float getRight() const { return m_position.x + (float)m_radius.x; }
    float getBottom() const { return m_position.y + (float)m_radius.y; }

    bool isOverlappingWith(const EntityHitBox &other) const {
        return (getLeft() < other.getRight() && getRight() > other.getLeft() &&
                getTop() < other.getBottom() && getBottom() > other.getTop());
    }

    void draw(Camera &cam) {
        cam.getScr().artist().drawRectangleRadius(
            {.x = static_cast<float>(m_radius.x),
             .y = static_cast<float>(m_radius.y)},
            {.x = m_position.x - cam.getPos().x,
             .y = m_position.y - cam.getPos().y},
            {.borderColor = artist_api::Color::fromARGB(0xAAFF00FF),
             .borderThickness = 1.f,
             .fillColor = artist_api::Color::fromARGB(0)});
    }

private:
    v2f &m_position;
    v2i m_radius;
};

template <typename T> class EntityComponent {
public:
    EntityComponent(T *component) : m_component(component) {}
    EntityComponent(T &component) : m_component(&component) {}

    bool exists() { return m_component != nullptr; }

    T &get() {
        if (m_component == nullptr)
            printf("Component does not exists");

        return *m_component;
    }

    static EntityComponent Empty() { return EntityComponent<T>(nullptr); }

private:
    T *m_component;
};

class EntitySolidBox {}; // TODO

static const int ENTITY_TAG_COUNT = 2;

enum class EntityTag { ENEMY, HARMFUL };

class EntityTags {
public:
    EntityTags &add(EntityTag tag) {
        m_tags[(int)tag] = true;
        return *this;
    }
    bool has(EntityTag tag) const { return m_tags[(int)tag]; }

    static const EntityTags &Empty() {
        static EntityTags tags;
        return tags;
    }

private:
    bool m_tags[ENTITY_TAG_COUNT] = {false};
};

#define DO_NOTHING_BY_DEFAULT
class Entity {
public:
    Entity() {}
    Entity(v2f _pos);

    // Basic methods
    virtual void init() { DO_NOTHING_BY_DEFAULT }
    virtual void update() { DO_NOTHING_BY_DEFAULT }
    virtual void draw(Camera &cam) { DO_NOTHING_BY_DEFAULT }

    // Required
    virtual EntityTypeID type() = 0;
    virtual const EntityTags &tags() { return EntityTags::Empty(); }

    // Componenets ( Which can be optional by EntityComponent wrapper )
    virtual EntityComponent<v2f> cradius() {
        return EntityComponent<v2f>::Empty();
    }
    virtual EntityComponent<v2f> cposition() {
        return EntityComponent<v2f>::Empty();
    }
    virtual EntityComponent<v2f> cspeed() {
        return EntityComponent<v2f>::Empty();
    }
    virtual EntityComponent<EntityHitBox> chitbox() {
        return EntityComponent<EntityHitBox>::Empty();
    }
    virtual EntityComponent<EntitySolidBox> csolidbox() {
        return EntityComponent<EntitySolidBox>::Empty();
    }

#define EXPOSE_TEMPLATE__(classname, methodname, propertyname)                 \
    EntityComponent<classname> methodname() override {                         \
        return EntityComponent<classname>(propertyname);                       \
    }

#define ENTITY_EXPOSE_RADIUS(propertyname)                                     \
    EXPOSE_TEMPLATE__(v2f, cradius, propertyname)
#define ENTITY_EXPOSE_POSITION(propertyname)                                   \
    EXPOSE_TEMPLATE__(v2f, cposition, propertyname)
#define ENTITY_EXPOSE_SPEED(propertyname)                                      \
    EXPOSE_TEMPLATE__(v2f, cspeed propertyname)
#define ENTITY_EXPOSE_HITBOX(propertyname)                                     \
    EXPOSE_TEMPLATE__(EntityHitBox, chitbox, propertyname)
#define ENTITY_EXPOSE_SOLIDBOX(propertyname)                                   \
    EXPOSE_TEMPLATE__(EntitySolidBox, csolidbox, propertyname)

    // Events
    virtual void onDestroy() { DO_NOTHING_BY_DEFAULT }
    virtual void onOutOfView() { DO_NOTHING_BY_DEFAULT }
    virtual void onHitboxCollision(Entity &entity) { DO_NOTHING_BY_DEFAULT }

    template <typename T> T &castTo() { return *((T *)this); }

    // Deprecated
    virtual void d_update() { }
    virtual void d_draw(Camera &cam) { }
    virtual void d_reactingToOthers(std::list<Entity *> &entities) { return; }
    void d_setPos(v2f pos) { this->dv_pos = pos; }
    v2f d_getPos() { return dv_pos; }
    void d_goToStartPos() { dv_pos = dv_startPos; }
    v2f d_getStartPos() { return dv_startPos; }
    void d_setHitBoxSize(v2f _size) { dv_hitBoxSize = _size; };
    v2f d_getHitBoxSize() { return dv_hitBoxSize; };
    bool d_entMeeting(Entity &ent, v2i meetSize);
    virtual void d_destroy() { dv_living = false; }
    uint8_t d_getType() { return dv_type; }
    bool d_isSolid() { return dv_solid; }
    bool d_isPlatform() { return dv_platform; }
    bool d_isPlatPushUp() { return dv_platPushUp; }
    bool d_isLiving() { return dv_living; };
    bool d_isInCamera(Camera &cam);
    bool d_collisionRight(Entity &ent, uint32_t shiftY = 0);
    bool d_collisionLeft(Entity &ent, uint32_t shiftY = 0);
    bool d_collisionTop(Entity &ent, uint32_t shiftY = 0);
    bool d_collisionBottom(Entity &ent, uint32_t shiftY = 0);
    bool d_collisionBottomPlatform(Entity &ent, uint32_t shiftY = 0);
    bool d_collisionMain(Entity &ent, uint32_t shiftY = 0);

protected:
    // Deprecated variables
    uint8_t dv_type = TYPE_UNKNOWN;
    bool dv_solid = false;
    bool dv_platform = false;
    bool dv_platPushUp = true;
    bool dv_living = true;
    v2f dv_pos;
    v2f dv_startPos;
    v2f dv_hitBoxSize = v2f(16, 16);
};
#undef NOTHING
