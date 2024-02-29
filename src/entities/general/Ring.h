#pragma once
#include "../Entity.h"
#include "AnimMgr.h"
#include "entities/Entity.h"
#include "entities/Player.h"
#include "entities/general/Sfx.h"
#include "entity-pool.hpp"
#include "game-loop-ticker.h"
#include "new-terrain.hpp"
#include "terrain-sensor.hpp"

class Ring : public Entity {
public:
    Ring(v2f pos, EntityPool& entities, terrain::Terrain &trn, float xsp = 0, float ysp = 0)
        : m_gndSensor(m_pos, terrain::SensorDirection::DOWN, trn)
        , m_entities(entities)
        , m_spd(xsp, ysp)
        , m_pos(pos)
    {
        m_bouncing  = (xsp != 0 || ysp != 0);
        m_liveTimer = 256;
    }

    static Ring *CreateRow(EntityPool& entityPool, terrain::Terrain &_trn,
                           v2f startPosition, int count, float direction = 0.f,
                           float spaceBetween = 0.f);

    ENTITY_EXPOSE_HITBOX(m_hitbox)

    void init()   override;
    void update() override;
    void draw(Camera &cam) override;

    EntityTypeID type() override { return EntityTypeID::RING; }

    bool isBounce() { return m_bouncing; }

    void onDestroy() override {
        AnimMgr rSfx;
        rSfx.create(TEX_OBJECTS);
        rSfx.set(84, 87, 0.5);

        m_entities.create(new SingleAnimationEffect(m_pos, rSfx, m_entities));
    }

    void onOutOfView() override {
        if (isBounce()) { /* TODO destroy */
        }
    }

    void onHitboxCollision(Entity &entity) override;

private:
    bool            m_bouncing = false;
    v2f             m_pos;
    v2f             m_spd = v2f(0, 0);
    EntityHitBox    m_hitbox = EntityHitBox(m_pos, v2i(6, 6));
    terrain::Sensor m_gndSensor;
    int             m_liveTimer = 0;
    AnimMgr         m_anim;
    EntityPool&     m_entities;

};