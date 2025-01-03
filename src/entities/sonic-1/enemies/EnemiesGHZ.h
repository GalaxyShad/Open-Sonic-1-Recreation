#pragma once

#include "../../general/Enemy.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "new-terrain.hpp"
#include "terrain-sensor.hpp"

#include <list>

#define MOTOBUG_SPD 1
#define BUZZ_SPD 4
#define CRAB_SPD 0.5

class EnMotobug : public Enemy {
public:
    EnMotobug(v2f pos, terrain::Terrain &terrain)
        : m_sensor(pos, terrain::SensorDirection::DOWN, terrain), Enemy(pos) {}
    void init();
    void d_update();
    void d_draw(Camera &cam);
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

private:
    terrain::Sensor m_sensor;

    void trnCollision();
    int dir = -1;
};

class EnChopper : public Enemy {
public:
    EnChopper(v2f _pos, artist_api::Animation &anim)
        : Enemy(_pos), animator_(anim) {}
    void init();
    void d_update();
    void draw(Camera &cam) override {

        auto &spr = animator_.getCurrentFrame();

        cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                               .y = dv_pos.y - cam.getPos().y});
    }

    void d_draw(Camera &cam) override {
        // nothing
    }

    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

private:
    artist_api::Animator animator_;
    float ysp = 0;
};

class EnCrab : public Enemy {
public:
    EnCrab(v2f pos, std::list<Entity *> &_entities, terrain::Terrain &terrain)
        : Enemy(pos), m_sensor(pos, terrain::SensorDirection::DOWN, terrain) {
        entities = &_entities;
    }
    void init();
    void d_update();
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

private:
    void trnCollision();
    float xsp = CRAB_SPD;
    int moveTimer = 0;
    bool faceRight = true;
    bool bulFlag = false;
    std::list<Entity *> *entities;

    int m_tick = 0;
    terrain::Sensor m_sensor;
};

class EnBuzz : public Enemy {
public:
    EnBuzz(v2f _pos) : Enemy(_pos) {}
    void init();
    void d_update();
    void d_draw(Camera &cam);
    void d_reactingToOthers(std::list<Entity *> &entities);
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

private:
    float xsp = BUZZ_SPD;
    int moveTimer = 0;
    int idleTimer = 8;
    int fireTimer = 0;
    bool faceRight = false;
    bool fired = false;
    AnimMgr animWings;
};

class Bullet : public Entity {
public:
    Bullet(v2f _pos, uint8_t _mode, int _dir = -1) : Entity(_pos) {
        mode = _mode;
        dir = _dir;
        init();
    }
    void init();
    void d_update();
    void d_draw(Camera &cam);
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

private:
    float ysp = 0;
    float xsp = 0;
    uint8_t mode = 0; // 0 - Buzz 1 - Crab
    int dir = -1;
    const float grv = 0.21875;
};
