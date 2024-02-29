#pragma once

#include "entities/Entity.h"

class Monitor : public Entity {
  public:
    enum Item { M_RINGS, M_INVINCIBILITY, M_LIVE, M_SHIELD, M_SPEED };

    Monitor(v2f _pos, Item item) : Entity(_pos), item(item) {}
    void init() {
        dv_solid = true;
        dv_platform = true;
        dv_type = TYPE_MONITOR;
        dv_hitBoxSize = v2f(32, 32);
        dv_anim.create(TEX_OBJECTS);
        dv_anim.set(56, 56, 0);

        animIcon.create(TEX_OBJECTS);
        animIcon.set(60 + item, 60 + item, 0);
    }
    void d_draw(Camera &cam) {
        cam.draw(dv_anim, dv_pos);
        cam.draw(animIcon, v2f(dv_pos.x, dv_pos.y - 3));
    }

    Item getItem() { return item; }

    EntityTypeID type() override { return EntityTypeID::MONITOR; }

  private:
    AnimMgr animIcon;
    Item item;
};

class BrokenMonitor : public Entity {
  public:
    BrokenMonitor(v2f _pos) : Entity(_pos) {
        dv_anim.create(TEX_OBJECTS);
        dv_anim.set(57, 57, 0);
    }
    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
};

class MonitorIcon : public Entity {
  public:
    MonitorIcon(v2f _pos, Monitor::Item item) : Entity(_pos), item(item) {
        init();
    }

    void init() {
        dv_pos.y -= 3;
        tick = 0;
        dv_anim.create(TEX_OBJECTS);
        dv_anim.set(60 + item, 60 + item, 0);
    }

    void d_update() {
        if (tick < 30)
            dv_pos.y -= 2;
        if (tick > 40)
            dv_living = false;

        tick++;
    }

    EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

  private:
    Monitor::Item item;
    int tick;
};