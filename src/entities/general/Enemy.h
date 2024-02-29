#pragma once

#include "../Entity.h"

class Enemy : public Entity
{
    public:
        Enemy(v2f _pos) : Entity(_pos) {}
        void setAbilityToDying(bool _canDie) { canDie = _canDie; };
        void onOutOfView() override {
            dv_pos = dv_startPos;
        }
    private:
        bool canDie = true;
};

class EnemyScore : public Entity {
    public:
        enum Points { P_100, P_200, P_500, P_1000 };
        EnemyScore(v2f _pos, Points points) : 
            Entity(_pos), points(points) { init(); }

        void init() {
            dv_pos.y -= 3;
            tick = 0;
            dv_anim.create(TEX_OBJECTS);
            dv_anim.set(70+points, 70+points, 0); 
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
        Points points;
        int tick;
};