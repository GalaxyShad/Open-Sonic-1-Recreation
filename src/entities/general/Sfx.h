#pragma once
#include "../Entity.h"
#include "AnimMgr.h"
#include "entity-pool.hpp"

class SingleAnimationEffect : public Entity {
public:
    SingleAnimationEffect(v2f pos, AnimMgr anim, EntityPool& pool) : m_pos(pos), m_anim(anim), m_pool(pool) {}
    void update() override {
        m_anim.tick();

        if (m_anim.getCurFrame() >= m_anim.getLastFrame()) {
            m_pool.destroy(*this);
        }
    }

    void draw(Camera &cam) override {
        cam.draw(m_anim, m_pos);
    }

    EntityTypeID type() override { return EntityTypeID::PARTICLE; }

private:
    v2f m_pos;
    AnimMgr m_anim;
    EntityPool& m_pool;
};