#pragma once

#include "../Entity.h"
#include "Terrain.h"

class Enemy : public Entity
{
    public:
        Enemy(v2f _pos) : Entity(_pos) {}
        void setAbilityToDying(bool _canDie) { canDie = _canDie; };
        virtual void trnCollision(Terrain& trn) {}
    private:
        bool canDie = true;
};

class EnemyScore : public Entity {
    public:
        enum Points { P_100, P_200, P_500, P_1000 };
        EnemyScore(v2f _pos, Points points) : 
            Entity(_pos), points(points) { create(); }

        void create() {
            pos.y -= 3;
            tick = 0;
            anim.create(TEX_OBJECTS);
            anim.set(70+points, 70+points, 0); 
        }

        void update() {
            if (tick < 30)  
                pos.y -= 2;
            if (tick > 40)
                living = false;

            tick++;
        }

    private:
        Points points;
        int tick;
};