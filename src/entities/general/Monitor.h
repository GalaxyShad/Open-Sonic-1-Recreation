#pragma once

#include "entities/Entity.h"

class Monitor : public Entity {
    public:
        enum Item {M_RINGS, M_INVINCIBILITY, M_LIVE, M_SHIELD, M_SPEED};

        Monitor(Vector2f _pos, Item item) : Entity(_pos), item(item) {}
        void create() {
            solid = true; 
            platform = true;
            type = TYPE_MONITOR; 
            hitBoxSize = Vector2f(32, 32); 
            anim.create(TEX_OBJECTS); 
            anim.set(56, 56, 0);


            animIcon.create(TEX_OBJECTS);
            animIcon.set(60+item, 60+item, 0); 
        }
        void draw(Camera& cam) { 
            cam.draw(anim, pos);
            cam.draw(animIcon, Vector2f(pos.x, pos.y-3)); 
        }

        Item getItem() { return item; }


    private:
        AnimMgr animIcon;
        Item item; 

};

class BrokenMonitor : public Entity {
    public:
        BrokenMonitor(Vector2f _pos) : Entity(_pos) { anim.create(TEX_OBJECTS); anim.set(57, 57, 0); }
};

class MonitorIcon : public Entity {
    public:
        MonitorIcon(Vector2f _pos, Monitor::Item item) : 
            Entity(_pos), item(item) { create(); }

        void create() {
            pos.y -= 3;
            tick = 0;
            anim.create(TEX_OBJECTS);
            anim.set(60+item, 60+item, 0); 
        }

        void update() {
            if (tick < 30)  
                pos.y -= 2;
            if (tick > 40)
                living = false;

            tick++;
        }

    private:
        Monitor::Item item;
        int tick;
};