#pragma once
#include "../Entity.h"
#include "new-terrain.hpp"
#include "terrain-sensor.hpp"

class Ring : public Entity {
  public:
    enum Dir { DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN };
    Ring(v2f _pos, terrain::Terrain &_trn)
        : m_gndSensor(v2f(0, 0), terrain::SensorDirection::DOWN, _trn),
          Entity(_pos) {}
    Ring(v2f _pos, terrain::Terrain &_trn, float _xsp, float _ysp)
        : m_gndSensor(v2f(0, 0), terrain::SensorDirection::DOWN, _trn),
          Entity(_pos) {
        xsp = _xsp;
        ysp = _ysp;
        bouncing = true;
        liveTimer = 256;
    }

    static Ring* CreateRow(std::list<Entity *> &ent, terrain::Terrain &_trn,
                           v2f startPosition, int count, float direction = 0.f,
                           float spaceBetween = 0.f);

    void init();
    void d_update();
    bool isBounce() { return bouncing; }
    void d_draw(Camera &cam);
    void animate(int frame);

  private:
    bool bouncing = false;
    float xsp = 0;
    float ysp = 0;
    int liveTimer = 0;
    terrain::Sensor m_gndSensor;
};